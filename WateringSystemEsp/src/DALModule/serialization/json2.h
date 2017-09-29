/*
 * json2.h
 *
 *  Created on: 2 баев„ 2017
 *      Author: IsM
 */

#ifndef SERIALIZATION_JSON2_H_
#define SERIALIZATION_JSON2_H_


#include <DALModule/serialization/cereal2.h>
#include <DALModule/serialization/traits2.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

#include "rapidjson/reader.h"

#include <WString.h>

//#include "rapidjson/istreamwrapper.h"
//#include "rapidjson/document.h"

#include <vector>

#include <stack>

#include <memory>//shared_pointer
#include <utility> //std::forward

namespace cereal2 {

using namespace rapidjson;

class JSONInputArchive: public InputArchive<JSONInputArchive>{

	using WString = String;

private:


    // const char * itsNextName;               //!< Next name set by NVP
     //rapidjson::IStreamWrapper itsReadStream;               //!< Rapidjson write stream
     //class Iterator;
    // std::vector<Iterator> itsIteratorStack; //!< 'Stack' of rapidJSON iterators
   //  rapidjson::Document itsDocument; //!< Rapidjson document

  //   typedef rapidjson::GenericValue<rapidjson::UTF8<>> JSONValue;
   //  typedef JSONValue::ConstMemberIterator MemberIterator;
   //  typedef JSONValue::ConstValueIterator ValueIterator;
  //   typedef rapidjson::Document::GenericValue GenericValue;

     //! An internal iterator that handles both array and object types
     /*! This class is a variant and holds both types of iterators that
         rapidJSON supports - one for arrays and one for objects. */
   /*  class Iterator
     {
       public:
         Iterator() : itsIndex( 0 ), itsType(Null_) {}

         Iterator(MemberIterator begin, MemberIterator end) :
           itsMemberItBegin(begin), itsMemberItEnd(end), itsIndex(0), itsType(Member)
         {
           if( std::distance( begin, end ) == 0 )
             itsType = Null_;
         }

         Iterator(ValueIterator begin, ValueIterator end) :
           itsValueItBegin(begin), itsValueItEnd(end), itsIndex(0), itsType(Value)
         {
           if( std::distance( begin, end ) == 0 )
             itsType = Null_;
         }

         //! Advance to the next node
         Iterator & operator++()
         {
           ++itsIndex;
           return *this;
         }

         //! Get the value of the current node
         GenericValue const & value()
         {
           switch(itsType)
           {
             case Value : return itsValueItBegin[itsIndex];
             case Member: return itsMemberItBegin[itsIndex].value;
          //   default: throw cereal::exception("JSONInputArchive internal error: null or empty iterator to object or array!");// $$$ need to replace this exception
           }
         }

         //! Get the name of the current node, or nullptr if it has no name
         const char * name() const
         {
           if( itsType == Member && (itsMemberItBegin + itsIndex) != itsMemberItEnd )
             return itsMemberItBegin[itsIndex].name.GetString();
           else
             return nullptr;
         }

         //! Adjust our position such that we are at the node with the given name
         //! @throws Exception if no such named node exists
         inline void search( const char * searchName )
         {
           const auto len = std::strlen( searchName );
           size_t index = 0;
           for( auto it = itsMemberItBegin; it != itsMemberItEnd; ++it, ++index )
           {
             const auto currentName = it->name.GetString();
             if( ( std::strncmp( searchName, currentName, len ) == 0 ) &&
                 ( std::strlen( currentName ) == len ) )
             {
               itsIndex = index;
               return;
             }
           }

        //   throw exception("JSON Parsing failed - provided NVP (" + std::string(searchName) + ") not found"); // $$$ how i pass the error?
         }

       private:
         MemberIterator itsMemberItBegin, itsMemberItEnd; //!< The member iterator (object)
         ValueIterator itsValueItBegin, itsValueItEnd;    //!< The value iterator (array)
         size_t itsIndex;                                 //!< The current index of this iterator
         enum Type {Value, Member, Null_} itsType;        //!< Whether this holds values (array) or members (objects) or nothing
     };*/

	struct MyHandler : public rapidjson::BaseReaderHandler<rapidjson::UTF8<>, MyHandler> {

		int lastInt=666;
		unsigned int lastUInt=666;
		WString lastString;
		std::vector<bool> isInsideArrayHistory = {false};




	    bool Null() {
	    	Serial.println(" called MyHandler Null");
	    	return true; }
	    bool Bool(bool ) {
	    	Serial.println(" called MyHandler Bool");
	    	return true; }
	    bool Int(int i) {
	    	lastInt = i;
	    	Serial.println(" called MyHandler Int");
	    	return true; }
	    bool Uint(unsigned u) {
	    	lastUInt = u;
	    	lastInt = u;
	    	Serial.println(" called MyHandler Uint");
	    	return true; }
	    bool Int64(int64_t ) {
	    	Serial.println(" called MyHandler Int64");
	    	return true; }
	    bool Uint64(uint64_t ) {
	    	Serial.println(" called MyHandler Uint64");
	    	return true; }
	    bool Double(double ) {
	    	Serial.println(" called MyHandler Double");
	    	return true; }
	    bool String(const Ch* str, SizeType length, bool copy) {
	    	Serial.println(" called MyHandler String");
	    	lastString = WString(str);
	        return true;
	    }
	    bool StartObject() {
	    	isInsideArrayHistory.push_back(false);
	    	Serial.println(" called MyHandler StartObject");
	    	return true; }
	    bool Key(const char* , SizeType , bool ) {
	    	Serial.println(" called MyHandler Key");
	        return true;
	    }
	    bool EndObject(SizeType ) {
	    	isInsideArrayHistory.pop_back();
	    	Serial.println(" called MyHandler EndObject");
	    	return true; }
	    bool StartArray() {
	    	Serial.println(" called MyHandler StartArray");
	    	isInsideArrayHistory.push_back(true);
	    	return true; }
	    bool EndArray(SizeType ) {
	    	Serial.println(" called MyHandler EndArray");
	    	isInsideArrayHistory.pop_back();
	    	return true; }
	};

	rapidjson::Reader reader;
	MyHandler handler;
	StringStream strStream;


public:
	/* JSONInputArchive(const char* json)://std::istream & stream) :
	        InputArchive<JSONInputArchive>(this),
	        itsNextName( nullptr )//,
	    //    itsReadStream(stream)
	      {
		 	Serial.println("before itsDocument.Parse(json);");
		    itsDocument.Parse(json);//("{\"a\": {\"x\":113}}");//itsDocument.ParseStream<>(itsReadStream);//itsDocument.Parse("{\"a\": {\"x\":113}}");//
		    Serial.println("end itsDocument.Parse");
		    if (itsDocument.IsArray())
	          itsIteratorStack.emplace_back(itsDocument.Begin(), itsDocument.End());
	        else
	          itsIteratorStack.emplace_back(itsDocument.MemberBegin(), itsDocument.MemberEnd());
	      }*/

	JSONInputArchive(StringStream ss) :  //std::istream & stream) :
			InputArchive<JSONInputArchive>(this),
			strStream(ss){
			init();

		/*while (!reader.IterativeParseComplete()) {
			reader.IterativeParseNext < kParseDefaultFlags > (strStream, handler);
			// Your handler has been called once.
		}*/
	}

	JSONInputArchive() :  //std::istream & stream) :
			InputArchive<JSONInputArchive>(this),
			strStream(NULL){
			//must call reset b4 use
	}

	void reset(const char* newStr){
		strStream = StringStream(newStr);
		init();
	}


private:
	void init(){
		handler = MyHandler();
		reader.IterativeParseInit();
		reader.IterativeParseNext < kParseDefaultFlags > (strStream, handler);//skip the first '{'
	}


    //! Searches for the expectedName node if it doesn't match the actualName
     /*! This needs to be called before every load or node start occurs.  This function will
         check to see if an NVP has been provided (with setNextName) and if so, see if that name matches the actual
         next name given.  If the names do not match, it will search in the current level of the JSON for that name.
         If the name is not found, an exception will be thrown.

         Resets the NVP name after called.

         @throws Exception if an expectedName is given and not found */
    /* inline void search()
     {
       // The name an NVP provided with setNextName()
       if( itsNextName )
       {
         // The actual name of the current node
         auto const actualName = itsIteratorStack.back().name();

         // Do a search if we don't see a name coming up, or if the names don't match
         if( !actualName || std::strcmp( itsNextName, actualName ) != 0 )
           itsIteratorStack.back().search( itsNextName );
       }

       itsNextName = nullptr;
     }*/

public:
      //! Starts a new node, going into its proper iterator
      /*! This places an iterator for the next node to be parsed onto the iterator stack.  If the next
          node is an array, this will be a value iterator, otherwise it will be a member iterator.

          By default our strategy is to start with the document root node and then recursively iterate through
          all children in the order they show up in the document.
          We don't need to know NVPs to do this; we'll just blindly load in the order things appear in.

          If we were given an NVP, we will search for it if it does not match our the name of the next node
          that would normally be loaded.  This functionality is provided by search(). */
     /* void startNode()
      {
        search();

        if(itsIteratorStack.back().value().IsArray())
          itsIteratorStack.emplace_back(itsIteratorStack.back().value().Begin(), itsIteratorStack.back().value().End());
        else
          itsIteratorStack.emplace_back(itsIteratorStack.back().value().MemberBegin(), itsIteratorStack.back().value().MemberEnd());
      }

      //! Finishes the most recently started node
      void finishNode()
      {
        itsIteratorStack.pop_back();
        ++itsIteratorStack.back();
      }

      //! Sets the name for the next node created with startNode
	void setNextName(const char * name) {
		itsNextName = name;
	}

    //! Loads a value from the current node - small signed overload
    template <class T, typename std::enable_if< std::is_signed<T>::value && sizeof(T) < sizeof(int64_t) ,  detail2::sfinae>::type = {}> inline
    void loadValue(T & val)
    {
      search();

      val = static_cast<T>( itsIteratorStack.back().value().GetInt() );
      ++itsIteratorStack.back();
    }*/

	//pre - the node is an object
	//moves the handler into the node e.g.- {"a":{"x":3}} will be {"a":{"x":3}}
	//where our pointer at-                  ^                          ^
	void enterNode(){
		if(handler.isInsideArrayHistory.back())//arrays objects dont have keys
			reader.IterativeParseNext < kParseDefaultFlags > (strStream, handler);// the start object ('{')
		else{
			reader.IterativeParseNext < kParseDefaultFlags > (strStream, handler);//the key (a in our example)
			reader.IterativeParseNext < kParseDefaultFlags > (strStream, handler);// the start object ('{')
		}
	}

	void exitNode(){
		reader.IterativeParseNext < kParseDefaultFlags > (strStream, handler);// the end object ('}')
	}

	void skipKey(){
		reader.IterativeParseNext < kParseDefaultFlags > (strStream, handler);//the key
	}

	void skipArrayStart(){
		if(handler.isInsideArrayHistory.back())//arrays objects dont have keys, maybe we array inside an array
			reader.IterativeParseNext < kParseDefaultFlags > (strStream, handler);//the '['
		else{
			reader.IterativeParseNext < kParseDefaultFlags > (strStream, handler);//the key (name of the array)
			reader.IterativeParseNext < kParseDefaultFlags > (strStream, handler);//the '['
		}
	}

	void skipArrayEnd(){
		reader.IterativeParseNext < kParseDefaultFlags > (strStream, handler);//the ']'
		}

	void loadValue(int& t){
		reader.IterativeParseNext < kParseDefaultFlags > (strStream, handler); //calls the next int
		t = handler.lastInt;
	}

	void loadValue(unsigned int& t){
			reader.IterativeParseNext < kParseDefaultFlags > (strStream, handler); //calls the next int
			t = handler.lastUInt;
		}

	void loadValue(WString& t){
				reader.IterativeParseNext < kParseDefaultFlags > (strStream, handler); //calls the next String
				t = handler.lastString;
			}

	template<class T,class A>
	void loadArray(std::vector<T,A> & vector ){
		while(strStream.Peek() != ']'){//while the next symbol is not the end of the array (meaning we still have more elemnts in the array)
			T t;

			operator ()(t);
			vector.push_back(std::move(t));
		}
	}



};











//######################################################### output archive #########################################################

class JSONOutputArchive: public OutputArchive<JSONOutputArchive>, public traits2::TextArchive {
	enum class NodeType {StartObject, InObject, StartArray, InArray};

	//using WriteStringBuffer = rapidjson::StringBuffer;
	//using JSONWriter = rapidjson::Writer<WriteStringBuffer>;

private:
	char const * itsNextName;            //!< The next name
	rapidjson::Writer<rapidjson::StringBuffer> itsWriter;                //!< Rapidjson writer
	std::stack<uint32_t> itsNameCounter; //!< Counter for creating unique names for unnamed nodes
	std::stack<NodeType> itsNodeStack;


public:

	//! Construct, outputting to the provided stream
	/*! @param stream The stream to output to.
	 @param options The JSON specific options to use.  See the Options struct
	 for the values of default parameters */
	JSONOutputArchive(rapidjson::StringBuffer& writer) :
			OutputArchive<JSONOutputArchive>(this), itsNextName(nullptr), itsWriter(writer) {
		init();
	}

	JSONOutputArchive() :
				OutputArchive<JSONOutputArchive>(this), itsNextName(nullptr) {
		//must call reset b4 use
		}

	void init(){
		itsNameCounter.push(0);
		itsNodeStack.push(NodeType::StartObject);
	}

	//! Destructor, flushes the JSON
	~JSONOutputArchive()
	{
		finalize();
	}

	void finalize(){
		if (itsNodeStack.top() == NodeType::InObject)
			itsWriter.EndObject();
		else if (itsNodeStack.top() == NodeType::InArray)
			itsWriter.EndArray();
		itsWriter.Flush();
	}

	void reset(rapidjson::StringBuffer& os){
		itsNameCounter.empty();
		itsNodeStack.empty();
		itsWriter.Reset(os);
		init();
	}

    //! @}
      /*! @name Internal Functionality
          Functionality designed for use by those requiring control over the inner mechanisms of
          the JSONOutputArchive */
      //! @{

      //! Starts a new node in the JSON output
      /*! The node can optionally be given a name by calling setNextName prior
          to creating the node

          Nodes only need to be started for types that are themselves objects or arrays */
      void startNode()
      {
        writeName();
        itsNodeStack.push(NodeType::StartObject);
        itsNameCounter.push(0);
      }

      //! Designates the most recently added node as finished
            void finishNode()
            {
              // if we ended up serializing an empty object or array, writeName
              // will never have been called - so start and then immediately end
              // the object/array.
              //
              // We'll also end any object/arrays we happen to be in
              switch(itsNodeStack.top())
              {
                case NodeType::StartArray:
                  itsWriter.StartArray();
                case NodeType::InArray:
                  itsWriter.EndArray();
                  break;
                case NodeType::StartObject:
                  itsWriter.StartObject();
                case NodeType::InObject:
                  itsWriter.EndObject();
                  break;
              }

              itsNodeStack.pop();
              itsNameCounter.pop();
            }

      //! Sets the name for the next node created with startNode
      void setNextName( const char * name )
      {
        itsNextName = name;
      }

      //! Saves a bool to the current node
      void saveValue(bool b)                { itsWriter.Bool(b);                                                         }
      //! Saves an int to the current node
      void saveValue(int i)                 { itsWriter.Int(i);                                                          }
      //! Saves a uint to the current node
      void saveValue(unsigned u)            { itsWriter.Uint(u);                                                         }
      //! Saves an int64 to the current node
      void saveValue(int64_t i64)           { itsWriter.Int64(i64);                                                      }
      //! Saves a uint64 to the current node
      void saveValue(uint64_t u64)          { itsWriter.Uint64(u64);                                                     }
      //! Saves a double to the current node
      void saveValue(double d)              { itsWriter.Double(d);                                                       }
      //! Saves a string to the current node
      void saveValue(std::string const & s) { itsWriter.String(s.c_str(), static_cast<rapidjson::SizeType>( s.size() )); }
      //! Saves a const char * to the current node
      void saveValue(char const * s)        { itsWriter.String(s);                                                       }
      //! Saves a nullptr to the current node
      void saveValue(std::nullptr_t)        { itsWriter.Null();                                                          }
      void saveValue(String const & s)      { itsWriter.String(s.c_str(), static_cast<rapidjson::SizeType>(s.length())); }


  	//! Write the name of the upcoming node and prepare object/array state
  	      /*! Since writeName is called for every value that is output, regardless of
  	          whether it has a name or not, it is the place where we will do a deferred
  	          check of our node state and decide whether we are in an array or an object.

  	          The general workflow of saving to the JSON archive is:

  	            1. (optional) Set the name for the next node to be created, usually done by an NVP
  	            2. Start the node
  	            3. (if there is data to save) Write the name of the node (this function)
  	            4. (if there is data to save) Save the data (with saveValue)
  	            5. Finish the node
  	          */
  	      void writeName()
  	      {
  	        NodeType const & nodeType = itsNodeStack.top();

  	        // Start up either an object or an array, depending on state
  	        if(nodeType == NodeType::StartArray)
  	        {
  	          itsWriter.StartArray();
  	          itsNodeStack.top() = NodeType::InArray;
  	        }
  	        else if(nodeType == NodeType::StartObject)
  	        {
  	          itsNodeStack.top() = NodeType::InObject;
  	          itsWriter.StartObject();
  	        }

  	        // Array types do not output names
  	        if(nodeType == NodeType::InArray) return;

  	        if(itsNextName == nullptr)
  	        {
  	        //  std::string name = "value" + std::to_string( itsNameCounter.top()++ ) + "\0";
  	        	Serial.print("in writeName() when itsNextName == nullptr : not suppose to be here!! ");
  	        //  saveValue(name); //the actual writing, i need to implement that
  	        	saveValue("someValue");
  	        }
  	        else
  	        {
  	        	saveValue(itsNextName); //the actual writing, i need to implement that
  	          itsNextName = nullptr;
  	        }
  	      }

  	      //! Designates that the current node should be output as an array, not an object
  	      void makeArray()//first writes the name of the array, then starts it
  	      {
  	    	writeName();
  	        itsNodeStack.push(NodeType::StartArray);
  	      }

  	      void endArray() {
  	    	finishNode();
  	      }




};// JSONOutputArchive

// ######################################################################
//! Prologue for NVPs for JSON archives
/*! NVPs do not start or finish nodes - they just set up the names */
template<class T> inline
void   prologue(JSONOutputArchive &, NameValuePair<T> const &) {
	//std::cout << "prologue for NameValuePair<T> " << std::endl;
}

//! Prologue for NVPs for JSON archives
template <class T> inline
void prologue( JSONInputArchive &, NameValuePair<T> const & )
{ }

// ######################################################################
//! Prologue for all other types for JSON archives (except minimal types)
/*! Starts a new node, named either automatically or by some NVP,
 that may be given data by the type about to be archived

 Minimal types do not start or finish nodes */

template<class T, typename std::enable_if< detail2::has_save_method<T, void(const detail2::OutputArchiveBase&)>::value ,  detail2::sfinae>::type = {}>
inline void   prologue(JSONOutputArchive & ar, T const &) {
	//std::cout << "prologue for classes with save method " << std::endl;
	ar.startNode();

	//std::cout << detail2::has_save_method<T, void(const detail2::OutputArchiveBase&)>::value << endl;
}

//! Prologue for all other types for JSON archives
template<class T, typename std::enable_if< detail2::has_load_method<T, void(const detail2::InputArchiveBase&)>::value ,  detail2::sfinae>::type = {}>
inline void prologue( JSONInputArchive & ar, T const & )
{
  Serial.println("inside prologue for things with load method");
  //ar.startNode();
	ar.enterNode();
}

// ######################################################################
//! Prologue for arithmetic types for JSON archives
template <class T, typename std::enable_if<(std::is_arithmetic<T>::value || is_same<T, String>::value),  detail2::sfinae>::type = {}> inline
void prologue( JSONOutputArchive & ar, T const & )
{
	//std::cout << "prologue for std::is_arithmetic<T>" << std::endl;
  ar.writeName();
}

//template <class T> inline
/*void prologue( JSONOutputArchive & ar, const String& )
{
	//std::cout << "prologue for std::is_arithmetic<T>" << std::endl;
  ar.writeName();
}*/

//! Prologue for arithmetic types for JSON archives
template <class T, typename std::enable_if<(std::is_arithmetic<T>::value || is_same<T, String>::value),  detail2::sfinae>::type = {}> inline
void prologue( JSONInputArchive & ar, T const & )
{
	ar.skipKey();
}

/*void prologue( JSONInputArchive & ar, String const & )
{
	ar.skipKey();
}*/

//! Serialization for non-arithmetic vector types
  template <class T, class A> inline
//  typename std::enable_if<!traits::is_output_serializable<BinaryData<T>, Archive>::value
  //                        || !std::is_arithmetic<T>::value, void>::type
  void   prologue(JSONOutputArchive & ar, std::vector<T, A> const &  )
  {
	  //cout << "prologue(JSONOutputArchive & ar, std::vector<T, A> const & vector" <<endl;
	  ar.makeArray();
   // ar( make_size_tag( static_cast<size_type>(vector.size()) ) ); // number of elements
   // for(auto && v : vector)
  //    ar( v );
  }

  //we just ignore the pointer part of the shared pointer (we just pass on what they are pointing on)
  template<class T> inline
  void   prologue(JSONOutputArchive &, std::shared_ptr<T> const &) {
  	//std::cout << "prologue for std::shared_ptr<T> " << std::endl;
  }



  //! Serialization for non-arithmetic vector types
    template <class T, class A> inline
  //  typename std::enable_if<!traits::is_output_serializable<BinaryData<T>, Archive>::value
    //                        || !std::is_arithmetic<T>::value, void>::type
    void prologue(JSONInputArchive & ar, std::vector<T, A> const &  )
    {
  	 // cout << "prologue(JSONOutputArchive & ar, std::vector<T, A> const & vector" <<endl;
  	  ar.skipArrayStart();
    }

    //we just ignore the pointer part of the shared pointer (we just pass on what they are pointing on)
    template<class T> inline
    void prologue(JSONInputArchive &, std::shared_ptr<T> const &) {
    	//std::cout << "prologue for std::shared_ptr<T> " << std::endl;
    }



// ######################################################################
//! Epilogue for NVPs for JSON archives
/*! NVPs do not start or finish nodes - they just set up the names */
template <class T> inline
void   epilogue( JSONOutputArchive &, NameValuePair<T> const & ) {
	//std::cout << "epilogue for NameValuePair<T> " << std::endl;
}

//! Epilogue for NVPs for JSON archives
/*! NVPs do not start or finish nodes - they just set up the names */
template <class T> inline
void epilogue( JSONInputArchive &, NameValuePair<T> const & )
{ }

template<class T, typename std::enable_if< detail2::has_save_method<T, void(const detail2::OutputArchiveBase&)>::value ,  detail2::sfinae>::type = {}>
inline void   epilogue(JSONOutputArchive & ar, T const &) {
	//std::cout << "epilogue for classes with save method" << std::endl;
	ar.finishNode();
	////std::cout << detail2::has_save_method<T, void(const detail2::OutputArchiveBase&)>::value << endl;
}

//! Epilogue for all other types other for JSON archives
template <class T,typename std::enable_if< detail2::has_load_method<T, void(const detail2::InputArchiveBase&)>::value ,  detail2::sfinae>::type = {}>
inline void epilogue( JSONInputArchive & ar, T const & )
{
  Serial.println("inside epilogue for things with load method");
  //ar.finishNode();
	ar.exitNode();
}

// ######################################################################
//! Epilogue for arithmetic types for JSON archives
template <class T, typename std::enable_if<std::is_arithmetic<T>::value || is_same<T, String>::value,  detail2::sfinae>::type = {}> inline
void   epilogue( JSONOutputArchive & , T const & )
{	//std::cout << "epilogue for is_arithmetic<T>" << std::endl;
	}


/*void   epilogue( JSONOutputArchive & , String const & )
{	//std::cout << "epilogue for is_arithmetic<T>" << std::endl;
	}*/

//! Epilogue for arithmetic types for JSON archives
template <class T, typename std::enable_if<std::is_arithmetic<T>::value || is_same<T, String>::value,  detail2::sfinae>::type = {}> inline
void epilogue( JSONInputArchive &, T const & )
{ }


//void epilogue( JSONInputArchive &, String const & ){ }


//! Serialization for non-arithmetic vector types
  template <class T, class A> inline
//  typename std::enable_if<!traits::is_output_serializable<BinaryData<T>, Archive>::value
  //                        || !std::is_arithmetic<T>::value, void>::type
  void   epilogue(JSONOutputArchive & ar, std::vector<T, A> const & )
  {
	  //cout << "epilogue(JSONOutputArchive & ar, std::vector<T, A" <<endl;
	  ar.endArray();//finish array node
	 // ar.itsNodeStack.pop();
   // ar( make_size_tag( static_cast<size_type>(vector.size()) ) ); // number of elements
  //  for(auto && v : vector)
  //    ar( v );
  }

  //we just ignore the pointer part of the shared pointer (we just pass on what they are pointing on)
  template<class T> inline
  void   epilogue(JSONOutputArchive &, std::shared_ptr<T> const &) {
  	//std::cout << "epilogue for std::shared_ptr<T> " << std::endl;
  }


  //! Serialization for non-arithmetic vector types
    template <class T, class A> inline
  //  typename std::enable_if<!traits::is_output_serializable<BinaryData<T>, Archive>::value
    //                        || !std::is_arithmetic<T>::value, void>::type
    void epilogue(JSONInputArchive & ar, std::vector<T, A> const &  )
    {
  	  //cout << "epilogue(JSONInputArchive & ar, std::vector<T, A" <<endl;
  	  ar.skipArrayEnd();//finish array node
  	 // ar.itsNodeStack.pop();
     // ar( make_size_tag( static_cast<size_type>(vector.size()) ) ); // number of elements
    //  for(auto && v : vector)
    //    ar( v );
    }

    //we just ignore the pointer part of the shared pointer (we just pass on what they are pointing on)
    template<class T> inline
    void epilogue(JSONInputArchive &, std::shared_ptr<T> const &) {
    	//std::cout << "epilogue for std::shared_ptr<T> " << std::endl;
    }



// ######################################################################
// Common JSONArchive serialization functions
// ######################################################################
//! Serializing NVP types to JSON
template<class T> inline
void   save(JSONOutputArchive & ar,NameValuePair<T> const & t) {
	//std::cout << "save for NameValuePair<T> " << std::endl;
	ar.setNextName(t.name.c_str());
	ar(t.value);
}

template <class T> inline
void load( JSONInputArchive & ar, NameValuePair<T> & t )
{
  Serial.println("inside load( JSONInputArchive & ar, NameValuePair<T> & t )");
 // ar.setNextName( t.name );
  ar( t.value );
}

//! Saving for arithmetic to JSON
  template <class T, typename std::enable_if<std::is_arithmetic<T>::value || std::is_same<T, String>::value,  detail2::sfinae>::type = {}> inline
  void save(JSONOutputArchive & ar, T const & t)
  {
	  //std::cout << "save for is_arithmetic<T> " << t << std::endl;
    ar.saveValue( t );
  }

/*  void save(JSONOutputArchive & ar, String const & t)
  {
	  //std::cout << "save for is_arithmetic<T> " << t << std::endl;
    ar.saveValue( t );
  }*/

  //! Loading arithmetic from JSON
  template <class T,  typename std::enable_if<(std::is_arithmetic<T>::value || std::is_same<T,String>::value),  detail2::sfinae>::type = {}> inline
  void load(JSONInputArchive & ar, T & t)
  {
    ar.loadValue( t );
  }

 /* void load(JSONInputArchive & ar, String & t)
  {
    ar.loadValue( t );
  }*/


  //! Serialization for non-arithmetic vector types
    template <class T, class A> inline
  //  typename std::enable_if<!traits::is_output_serializable<BinaryData<T>, Archive>::value
    //                        || !std::is_arithmetic<T>::value, void>::type
    void save( JSONOutputArchive & ar, std::vector<T, A> const & vector )
    {
    	//std::cout << "save for std::vector<T, A> " << std::endl;
     // ar( make_size_tag( static_cast<size_type>(vector.size()) ) ); // number of elements
      for(auto && v : vector)
        ar( v );
    }

    //we just ignore the pointer part of the shared pointer (we just pass on what they are pointing on)
    template<class T> inline
    void save(JSONOutputArchive & ar, std::shared_ptr<T> const & sharedPointer) {
    	ar(*sharedPointer.get());
    }


    //! Serialization for non-arithmetic vector types
      template < class T, class A> inline
    //  typename std::enable_if<!traits::is_output_serializable<BinaryData<T>, Archive>::value
      //                        || !std::is_arithmetic<T>::value, void>::type
      void load( JSONInputArchive & ar, std::vector<T, A> & vector )
      {
      //	std::cout << "load for std::vector<T, A> " << std::endl;
      	ar.loadArray(vector);
       // ar( make_size_tag( static_cast<size_type>(vector.size()) ) ); // number of elements
     //   for(auto && v : vector)
     //     ar( v );
      }

      //we just ignore the pointer part of the shared pointer (we just pass on what they are pointing on)
      template<class T> inline
      void load(JSONInputArchive & ar, std::shared_ptr<T> & sharedPointer) {
    	  std::shared_ptr<T> t = make_shared<T>();
    	  ar(*t);
    	 // make_shared<T>();
    	  sharedPointer = t;//shared_ptr<T>(std::move(t));
      }


} /* namespace cereal2 */

#endif /* SERIALIZATION_JSON2_H_ */
