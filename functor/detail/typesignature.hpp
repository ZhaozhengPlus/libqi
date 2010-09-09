/*
** Author(s):
**  - Cedric GESTES <gestes@aldebaran-robotics.com>
**
** Copyright (C) 2010 Aldebaran Robotics
*/

#ifndef     AL_MESSAGING_DETAIL_TYPESIGNATURE_HPP_
# define     AL_MESSAGING_DETAIL_TYPESIGNATURE_HPP_

# include <string>
# include <vector>
# include <map>
# include <boost/mpl/string.hpp>
# include <boost/utility.hpp>
# include <boost/function_types/is_function_pointer.hpp>
# include <boost/function_types/is_function.hpp>
namespace AL {
  namespace detail {


    template <typename T, class Enable = void>
    struct signature {
      typedef boost::mpl::string<'U', 'N', 'K', 'N', 'O', 'W', 'N'> value;
    };

    template <>
    struct signature<void>  {
      typedef boost::mpl::string<'v'> value;
    };

    template <>
    struct signature<bool>  {
      typedef boost::mpl::string<'b'> value;
    };

    template <>
    struct signature<char> {
      typedef boost::mpl::string<'c'> value;
    };

    template <>
    struct signature<int> {
      typedef boost::mpl::string<'i'> value;
    };

    template <>
    struct signature<float> {
      typedef boost::mpl::string<'f'> value;
    };

    template <>
    struct signature<double> {
      typedef boost::mpl::string<'d'> value;
    };

    template <>
    struct signature<std::string> {
      typedef boost::mpl::string<'s'> value;
    };

    //STL
    template <typename U>
    struct signature< std::vector<U> > {
    private:
      typedef typename boost::mpl::copy<typename signature<U>::value,
                                        boost::mpl::back_inserter< boost::mpl::string<'['>::type >
                                        >::type value0;
    public:
      typedef typename boost::mpl::copy<boost::mpl::string<']'>::type,
                                        boost::mpl::back_inserter< value0 >
                                        >::type value;
    };

    template <typename T1, typename T2>
    struct signature< std::map<T1, T2> > {
    private:
      typedef typename boost::mpl::copy<typename signature<T1>::value,
                                        boost::mpl::back_inserter< boost::mpl::string<'{'>::type >
                                        >::type value0;
    typedef typename boost::mpl::copy<typename signature<T2>::value,
                                      boost::mpl::back_inserter< value0 >
                                      >::type value1;
    public:
      typedef typename boost::mpl::copy<boost::mpl::string<'}'>::type,
                                        boost::mpl::back_inserter< value1 >
                                        >::type value;
    };


    //TYPE QUALIFIER
    template <typename T>
    struct signature<T*, typename boost::disable_if< boost::function_types::is_function<T> >::type> {
      typedef typename boost::mpl::copy<boost::mpl::string<'*'>::type,
                                        boost::mpl::back_inserter< typename signature<T>::value >
                                        >::type value;
    };

    template <typename T>
    struct signature<T&> {
      typedef typename signature<T>::value value;
//    typedef typename boost::mpl::copy<boost::mpl::string<'&'>::type,
//                                      boost::mpl::back_inserter< typename signature<T>::value >
//                                     >::type value;
    };

    template <typename T>
    struct signature<const T> {
      typedef typename signature<T>::value value;
//    typedef typename boost::mpl::copy<boost::mpl::string<'#'>::type,
//                                      boost::mpl::back_inserter< typename signature<T>::value >
//                                     >::type value;
    };

  }
}

#endif      /* !AL_MESSAGING_DETAIL_TYPESIGNATURE_HPP_ */
