/*
**  Copyright (C) 2012 Aldebaran Robotics
**  See COPYING for the license
*/
#include <map>
#include <qi/atomic.hpp>

#include <boost/thread/recursive_mutex.hpp>

#include <qitype/signal.hpp>
#include <qitype/genericvalue.hpp>

#include "src/object_p.hpp"
#include "src/signal_p.hpp"

namespace qi {


  static qi::atomic<long> linkUid = 1;

  void SignalBase::operator()(
      qi::AutoGenericValue p1,
      qi::AutoGenericValue p2,
      qi::AutoGenericValue p3,
      qi::AutoGenericValue p4,
      qi::AutoGenericValue p5,
      qi::AutoGenericValue p6,
      qi::AutoGenericValue p7,
      qi::AutoGenericValue p8)
  {
    qi::AutoGenericValue* vals[8]= {&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8};
    std::vector<qi::GenericValue> params;
    for (unsigned i=0; i<8; ++i)
      if (vals[i]->value)
        params.push_back(*vals[i]);
    // Signature construction
    std::string signature = "(";
    for (unsigned i=0; i< params.size(); ++i)
      signature += params[i].signature();
    signature += ")";
    if (signature != _p->signature)
    {
      qiLogError("qi.signal") << "Dropping emit: signature mismatch: " << signature <<" " << _p->signature;
      return;
    }
    trigger(params);
  }

  void SignalBase::trigger(const GenericFunctionParameters& params)
  {
    if (!_p)
      return;
    boost::recursive_mutex::scoped_lock sl(_p->mutex);
    SignalSubscriberMap::iterator i;
    for (i=_p->subscriberMap.begin(); i!= _p->subscriberMap.end(); ++i)
      i->second.call(params);
  }

  class FunctorCall
  {
  public:
    FunctorCall(GenericFunctionParameters& params,
      SignalSubscriber* sub)
    : sub(sub)
    {
      std::swap((std::vector<GenericValue>&)this->params, (std::vector<GenericValue>&)params);
    }
    FunctorCall(const FunctorCall& b)
    {
      *this = b;
    }
    void operator=(const FunctorCall& b)
    {
      std::swap((std::vector<GenericValue>&)(this->params),
        (std::vector<GenericValue>&)b.params);
      sub = b.sub;
    }
    GenericFunctionParameters params;
    SignalSubscriber* sub;
    void operator() ()
    {
      if (sub->enabled)
        sub->handler(params);
      --sub->active;
      params.destroy();
    }
  };
  void SignalSubscriber::call(const GenericFunctionParameters& args)
  {
    if (!enabled)
      return;
    if (handler.type)
    {
      if (eventLoop)
      {
        ++active;
        // Event emission is always asynchronous
        GenericFunctionParameters copy = args.copy();
        eventLoop->asyncCall(0, FunctorCall(copy, this));
      }
      else
        handler(args);
    }
    if (target)
      target->metaEmit(method, args);
  }

  SignalBase::Link SignalBase::connect(GenericFunction callback, EventLoop* ctx)
  {
    return connect(SignalSubscriber(callback, ctx));
  }

  SignalBase::Link SignalBase::connect(qi::ObjectPtr o, unsigned int slot)
  {
    return connect(SignalSubscriber(o, slot));
  }

  SignalBase::Link SignalBase::connect(const SignalSubscriber& src)
  {
    if (!_p)
    {
      _p = boost::shared_ptr<SignalBasePrivate>(new SignalBasePrivate());
    }
    boost::recursive_mutex::scoped_lock sl(_p->mutex);
    Link res = ++linkUid;
    _p->subscriberMap[res] = src;
    SignalSubscriber& s = _p->subscriberMap[res];
    s.linkId = res;
    s.source = this;
    if (s.target)
    {
      ObjectPtr o = s.target;
      Manageable* m = o->type->manageable(o->value);
      if (m)
        m->addRegistration(s);
    }
    return res;
  }

  SignalBase::SignalBase(const std::string& sig)
    : _p(new SignalBasePrivate)
  {
    _p->signature = sig;
  }

  SignalBase::SignalBase()
  {
  }

  SignalBase::SignalBase(const SignalBase& b)
  {
    (*this) = b;
  }

  SignalBase& SignalBase::operator = (const SignalBase& b)
  {
    if (!b._p)
    {
      const_cast<SignalBase&>(b)._p = boost::shared_ptr<SignalBasePrivate>(new SignalBasePrivate());
    }
    _p = b._p;
    return *this;
  }

  std::string SignalBase::signature() const
  {
    return _p?_p->signature:"";
  }

  bool SignalBasePrivate::disconnect(const SignalBase::Link& l)
  {
    boost::recursive_mutex::scoped_lock sl(mutex);
    SignalSubscriberMap::iterator it = subscriberMap.find(l);
    if (it == subscriberMap.end())
      return false;
    SignalSubscriber& s = it->second;
    // Ensure no call on subscriber occurrs once this function returns
    s.enabled = false;
    while (*s.active)
      os::msleep(1); // FIXME too long
    ObjectPtr target = s.target;
    subscriberMap.erase(it);
    if (target)
    {
      Manageable* m = target->type->manageable(target->value);
      if (m)
        m->removeRegistration(l);
    }
    return true;
  }

  bool SignalBase::disconnect(const Link &link) {
    if (!_p)
      return false;
    else
      return _p->disconnect(link);
  }

  SignalBase::~SignalBase()
  {
    if (!_p)
      return;
    SignalSubscriberMap::iterator i;
    std::vector<Link> links;
    for (i = _p->subscriberMap.begin(); i!= _p->subscriberMap.end(); ++i)
    {
      links.push_back(i->first);
    }
    for (unsigned i=0; i<links.size(); ++i)
      disconnect(links[i]);

  }

  std::vector<SignalSubscriber> SignalBase::subscribers()
  {

    std::vector<SignalSubscriber> res;
    if (!_p)
      return res;
    boost::recursive_mutex::scoped_lock sl(_p->mutex);
    SignalSubscriberMap::iterator i;
    for (i = _p->subscriberMap.begin(); i!= _p->subscriberMap.end(); ++i)
      res.push_back(i->second);
    return res;
  }

  void SignalBasePrivate::reset() {
    boost::recursive_mutex::scoped_lock sl(mutex);
    SignalSubscriberMap::iterator it = subscriberMap.begin();
    while (it != subscriberMap.end()) {
      disconnect(it->first);
      it = subscriberMap.begin();
    }
  }

}
