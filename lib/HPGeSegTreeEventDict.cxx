// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME HPGeSegTreeEventDict
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Header files passed as explicit arguments
#include "HPGeSegTreeEvent.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_HPGeSegTreeEvent(void *p = nullptr);
   static void *newArray_HPGeSegTreeEvent(Long_t size, void *p);
   static void delete_HPGeSegTreeEvent(void *p);
   static void deleteArray_HPGeSegTreeEvent(void *p);
   static void destruct_HPGeSegTreeEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::HPGeSegTreeEvent*)
   {
      ::HPGeSegTreeEvent *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::HPGeSegTreeEvent >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("HPGeSegTreeEvent", ::HPGeSegTreeEvent::Class_Version(), "HPGeSegTreeEvent.h", 10,
                  typeid(::HPGeSegTreeEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::HPGeSegTreeEvent::Dictionary, isa_proxy, 4,
                  sizeof(::HPGeSegTreeEvent) );
      instance.SetNew(&new_HPGeSegTreeEvent);
      instance.SetNewArray(&newArray_HPGeSegTreeEvent);
      instance.SetDelete(&delete_HPGeSegTreeEvent);
      instance.SetDeleteArray(&deleteArray_HPGeSegTreeEvent);
      instance.SetDestructor(&destruct_HPGeSegTreeEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::HPGeSegTreeEvent*)
   {
      return GenerateInitInstanceLocal((::HPGeSegTreeEvent*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::HPGeSegTreeEvent*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr HPGeSegTreeEvent::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *HPGeSegTreeEvent::Class_Name()
{
   return "HPGeSegTreeEvent";
}

//______________________________________________________________________________
const char *HPGeSegTreeEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::HPGeSegTreeEvent*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int HPGeSegTreeEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::HPGeSegTreeEvent*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *HPGeSegTreeEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::HPGeSegTreeEvent*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *HPGeSegTreeEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::HPGeSegTreeEvent*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void HPGeSegTreeEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class HPGeSegTreeEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(HPGeSegTreeEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(HPGeSegTreeEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_HPGeSegTreeEvent(void *p) {
      return  p ? new(p) ::HPGeSegTreeEvent : new ::HPGeSegTreeEvent;
   }
   static void *newArray_HPGeSegTreeEvent(Long_t nElements, void *p) {
      return p ? new(p) ::HPGeSegTreeEvent[nElements] : new ::HPGeSegTreeEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_HPGeSegTreeEvent(void *p) {
      delete ((::HPGeSegTreeEvent*)p);
   }
   static void deleteArray_HPGeSegTreeEvent(void *p) {
      delete [] ((::HPGeSegTreeEvent*)p);
   }
   static void destruct_HPGeSegTreeEvent(void *p) {
      typedef ::HPGeSegTreeEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::HPGeSegTreeEvent

namespace ROOT {
   static TClass *vectorlEHPGeSegTreeEventgR_Dictionary();
   static void vectorlEHPGeSegTreeEventgR_TClassManip(TClass*);
   static void *new_vectorlEHPGeSegTreeEventgR(void *p = nullptr);
   static void *newArray_vectorlEHPGeSegTreeEventgR(Long_t size, void *p);
   static void delete_vectorlEHPGeSegTreeEventgR(void *p);
   static void deleteArray_vectorlEHPGeSegTreeEventgR(void *p);
   static void destruct_vectorlEHPGeSegTreeEventgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<HPGeSegTreeEvent>*)
   {
      vector<HPGeSegTreeEvent> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<HPGeSegTreeEvent>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<HPGeSegTreeEvent>", -2, "vector", 389,
                  typeid(vector<HPGeSegTreeEvent>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEHPGeSegTreeEventgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<HPGeSegTreeEvent>) );
      instance.SetNew(&new_vectorlEHPGeSegTreeEventgR);
      instance.SetNewArray(&newArray_vectorlEHPGeSegTreeEventgR);
      instance.SetDelete(&delete_vectorlEHPGeSegTreeEventgR);
      instance.SetDeleteArray(&deleteArray_vectorlEHPGeSegTreeEventgR);
      instance.SetDestructor(&destruct_vectorlEHPGeSegTreeEventgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<HPGeSegTreeEvent> >()));

      ::ROOT::AddClassAlternate("vector<HPGeSegTreeEvent>","std::vector<HPGeSegTreeEvent, std::allocator<HPGeSegTreeEvent> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<HPGeSegTreeEvent>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEHPGeSegTreeEventgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<HPGeSegTreeEvent>*)nullptr)->GetClass();
      vectorlEHPGeSegTreeEventgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEHPGeSegTreeEventgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEHPGeSegTreeEventgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<HPGeSegTreeEvent> : new vector<HPGeSegTreeEvent>;
   }
   static void *newArray_vectorlEHPGeSegTreeEventgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<HPGeSegTreeEvent>[nElements] : new vector<HPGeSegTreeEvent>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEHPGeSegTreeEventgR(void *p) {
      delete ((vector<HPGeSegTreeEvent>*)p);
   }
   static void deleteArray_vectorlEHPGeSegTreeEventgR(void *p) {
      delete [] ((vector<HPGeSegTreeEvent>*)p);
   }
   static void destruct_vectorlEHPGeSegTreeEventgR(void *p) {
      typedef vector<HPGeSegTreeEvent> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<HPGeSegTreeEvent>

namespace {
  void TriggerDictionaryInitialization_HPGeSegTreeEventDict_Impl() {
    static const char* headers[] = {
"HPGeSegTreeEvent.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/vincent/root/root_install/include/",
"/home/vincent/EliadeSorting/lib/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "HPGeSegTreeEventDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$HPGeSegTreeEvent.h")))  HPGeSegTreeEvent;
namespace std{template <typename _Tp> class __attribute__((annotate("$clingAutoload$bits/allocator.h")))  __attribute__((annotate("$clingAutoload$string")))  allocator;
}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "HPGeSegTreeEventDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "HPGeSegTreeEvent.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"HPGeSegTreeEvent", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("HPGeSegTreeEventDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_HPGeSegTreeEventDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_HPGeSegTreeEventDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_HPGeSegTreeEventDict() {
  TriggerDictionaryInitialization_HPGeSegTreeEventDict_Impl();
}
