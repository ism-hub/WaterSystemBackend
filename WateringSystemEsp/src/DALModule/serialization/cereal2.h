/*
 * cereal2.h
 *
 *  Created on: 31 αιεμι 2017
 *      Author: IsM
 */

#ifndef SERIALIZATION_CEREAL2_H_
#define SERIALIZATION_CEREAL2_H_

#include <DALModule/serialization/helpers2.h>
#include <DALModule/serialization/traits2.h>
#include <utility>
#include <cstdint>



using namespace std;
namespace cereal2 {


//! The base input archive class
  /*! This is the base input archive for all input archives.  If you create
      a custom archive class, it should derive from this, passing itself as
      a template parameter for the ArchiveType.

      The base class provides all of the functionality necessary to
      properly forward data to the correct serialization functions.

      Individual archives should use a combination of prologue and
      epilogue functions together with specializations of serialize, save,
      and load to alter the functionality of their serialization.

      @tparam ArchiveType The archive type that derives from InputArchive
      @tparam Flags Flags to control advanced functionality.  See the Flags
                    enum for more information.
      @ingroup Internal */
template<class ArchiveType, std::uint32_t Flags = 0>
class InputArchive: public detail2::InputArchiveBase {

private:
     ArchiveType * const self;

public:
      //! Construct the output archive
      /*! @param derived A pointer to the derived ArchiveType (pass this from the derived archive) */
      InputArchive(ArchiveType * const derived) :
        self(derived)
    //    itsBaseClassSet(),
    //    itsSharedPointerMap(),
    //    itsPolymorphicTypeMap(),
   //     itsVersionedTypes()
      { }

	//! Serializes all passed in data
	/*! This is the primary interface for serializing data with an archive */
	template<class ... Types> inline ArchiveType & operator()(Types && ... args) {
		process(std::forward<Types>( args )...);
		return *self;
	}

private:
      //! Serializes data after calling prologue, then calls epilogue
      template <class T> inline
      void process( T && head )
      {
        prologue( *self, head );
        self->processImpl( head );
        epilogue( *self, head );
      }

      //! Unwinds to process all data
      template <class T, class ... Other> inline
      void process( T && head, Other && ... tail )
      {
        process( std::forward<T>( head ) );
        process( std::forward<Other>( tail )... );
      }

      template<class Archive, class T> inline
      static auto member_load(Archive & ar, T & t) -> decltype(t.load(ar))
      { return t.load(ar); }

      //! process if has load method
      template<class T, typename std::enable_if< detail2::has_load_method<T, void(const detail2::InputArchiveBase&)>::value ,  detail2::sfinae>::type = {}> inline
      ArchiveType & processImpl(T & t)
      {
        member_load(*self, t);
        return *self;
      }

      //! process for things which don't have member load
      template<typename T, typename std::enable_if< !detail2::has_load_method<T, void(const detail2::InputArchiveBase&)>::value ,  detail2::sfinae>::type = {}> inline
      ArchiveType & processImpl(T & t)
      {
		load(*self, t);
		return *self;
      }


};




























template<class ArchiveType, std::uint32_t Flags = 0>
class OutputArchive: public detail2::OutputArchiveBase {

public:
	//! Construct the output archive
	/*! @param derived A pointer to the derived ArchiveType (pass this from the derived archive) */
	OutputArchive(ArchiveType * const derived) :self(derived), itsCurrentPointerId(1), itsCurrentPolymorphicTypeId(1) {
	}

	//! Serializes all passed in data
	/*! This is the primary interface for serializing data with an archive */
	template<class ... Types>
	inline ArchiveType & operator()(Types && ... args) {
		self->process(std::forward<Types>( args )...);
		return *self;
	}

private:
	ArchiveType * const self;
	//! The id to be given to the next pointer
	std::uint32_t itsCurrentPointerId;
	//! The id to be given to the next polymorphic type name
	std::uint32_t itsCurrentPolymorphicTypeId;


	//! Serializes data after calling prologue, then calls epilogue
	template<class T> inline
	void process(T && head) {
		prologue(*self, head);
		self->processImpl(head);
		epilogue(*self, head);
	}

    //! Unwinds to process all data
    template <class T, class ... Other> inline
    void process( T && head, Other && ... tail )
    {
      self->process( std::forward<T>( head ) );
      self->process( std::forward<Other>( tail )... );
    }

    enum class enabler{};

    template<class Cond, class T = enabler>
    using EnableIf = typename std::enable_if<Cond::value, T>::type;

    //! NameValuePair handle
  /*  template <typename T, typename std::enable_if< std::is_base_of<detail2::NameValuePairCore, T>::value ,  detail2::sfinae>::type = detail2::sfinae2> inline
    ArchiveType & processImpl(T const & t)
    {
    //  save(*self, t);
    	cout << "processImpl2 for NameValuePair<T> " << endl;
      return *self;
    }*/


    template<class Archive, class T> inline
    static auto  member_save(Archive & ar, T const & t) -> decltype(t.save(ar))
    { //cout << "Inside member_save " << endl;
    	return t.save(ar); }

    //! process if has save method
    template<class T, typename std::enable_if< detail2::has_save_method<T, void(const detail2::OutputArchiveBase&)>::value ,  detail2::sfinae>::type = {}> inline
	ArchiveType &  processImpl(T const & t) {
    	//cout << "processImpl for classes with save method " << endl;
    	member_save(*self, t);
		return *self;
	}

    //! process for things which don't have member save
	template<typename T, typename std::enable_if< !detail2::has_save_method<T, void(const detail2::OutputArchiveBase&)>::value ,  detail2::sfinae>::type = {}> inline
	ArchiveType &  processImpl(T const & t) {
		//cout << "process for things which don't have member save " << endl;
		save(*self, t);
		return *self;
	}

};



template<class T> inline
NameValuePair<T> make_nvp(const char *name, T&& value){
	return {name, std::forward<T>(value)};
}

#define CEREAL2_NVP(T) ::cereal2::make_nvp(#T, T)


} /* namespace cereal2 */

#endif /* SERIALIZATION_CEREAL2_H_ */
