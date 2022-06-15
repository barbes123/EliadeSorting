// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME HPGeTreeEventDict
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

// The generated code does not explicitly qualifies STL entities
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "HPGeTreeEvent.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_HPGeTreeEvent(void *p = 0);
   static void *newArray_HPGeTreeEvent(Long_t size, void *p);
   static void delete_HPGeTreeEvent(void *p);
   static void deleteArray_HPGeTreeEvent(void *p);
   static void destruct_HPGeTreeEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::HPGeTreeEvent*)
   {
      ::HPGeTreeEvent *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::HPGeTreeEvent >(0);
      static ::ROOT::TGenericClassInfo 
         instance("HPGeTreeEvent", ::HPGeTreeEvent::Class_Version(), "HPGeTreeEvent.h", 10,
                  typeid(::HPGeTreeEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::HPGeTreeEvent::Dictionary, isa_proxy, 4,
                  sizeof(::HPGeTreeEvent) );
      instance.SetNew(&new_HPGeTreeEvent);
      instance.SetNewArray(&newArray_HPGeTreeEvent);
      instance.SetDelete(&delete_HPGeTreeEvent);
      instance.SetDeleteArray(&deleteArray_HPGeTreeEvent);
      instance.SetDestructor(&destruct_HPGeTreeEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::HPGeTreeEvent*)
   {
      return GenerateInitInstanceLocal((::HPGeTreeEvent*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::HPGeTreeEvent*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr HPGeTreeEvent::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *HPGeTreeEvent::Class_Name()
{
   return "HPGeTreeEvent";
}

//______________________________________________________________________________
const char *HPGeTreeEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::HPGeTreeEvent*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int HPGeTreeEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::HPGeTreeEvent*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *HPGeTreeEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::HPGeTreeEvent*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *HPGeTreeEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::HPGeTreeEvent*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void HPGeTreeEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class HPGeTreeEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(HPGeTreeEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(HPGeTreeEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_HPGeTreeEvent(void *p) {
      return  p ? new(p) ::HPGeTreeEvent : new ::HPGeTreeEvent;
   }
   static void *newArray_HPGeTreeEvent(Long_t nElements, void *p) {
      return p ? new(p) ::HPGeTreeEvent[nElements] : new ::HPGeTreeEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_HPGeTreeEvent(void *p) {
      delete ((::HPGeTreeEvent*)p);
   }
   static void deleteArray_HPGeTreeEvent(void *p) {
      delete [] ((::HPGeTreeEvent*)p);
   }
   static void destruct_HPGeTreeEvent(void *p) {
      typedef ::HPGeTreeEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::HPGeTreeEvent

namespace ROOT {
   static TClass *vectorlEHPGeTreeEventgR_Dictionary();
   static void vectorlEHPGeTreeEventgR_TClassManip(TClass*);
   static void *new_vectorlEHPGeTreeEventgR(void *p = 0);
   static void *newArray_vectorlEHPGeTreeEventgR(Long_t size, void *p);
   static void delete_vectorlEHPGeTreeEventgR(void *p);
   static void deleteArray_vectorlEHPGeTreeEventgR(void *p);
   static void destruct_vectorlEHPGeTreeEventgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<HPGeTreeEvent>*)
   {
      vector<HPGeTreeEvent> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<HPGeTreeEvent>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<HPGeTreeEvent>", -2, "vector", 386,
                  typeid(vector<HPGeTreeEvent>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEHPGeTreeEventgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<HPGeTreeEvent>) );
      instance.SetNew(&new_vectorlEHPGeTreeEventgR);
      instance.SetNewArray(&newArray_vectorlEHPGeTreeEventgR);
      instance.SetDelete(&delete_vectorlEHPGeTreeEventgR);
      instance.SetDeleteArray(&deleteArray_vectorlEHPGeTreeEventgR);
      instance.SetDestructor(&destruct_vectorlEHPGeTreeEventgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<HPGeTreeEvent> >()));

      ::ROOT::AddClassAlternate("vector<HPGeTreeEvent>","std::vector<HPGeTreeEvent, std::allocator<HPGeTreeEvent> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<HPGeTreeEvent>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEHPGeTreeEventgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<HPGeTreeEvent>*)0x0)->GetClass();
      vectorlEHPGeTreeEventgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEHPGeTreeEventgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEHPGeTreeEventgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<HPGeTreeEvent> : new vector<HPGeTreeEvent>;
   }
   static void *newArray_vectorlEHPGeTreeEventgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<HPGeTreeEvent>[nElements] : new vector<HPGeTreeEvent>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEHPGeTreeEventgR(void *p) {
      delete ((vector<HPGeTreeEvent>*)p);
   }
   static void deleteArray_vectorlEHPGeTreeEventgR(void *p) {
      delete [] ((vector<HPGeTreeEvent>*)p);
   }
   static void destruct_vectorlEHPGeTreeEventgR(void *p) {
      typedef vector<HPGeTreeEvent> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<HPGeTreeEvent>

namespace {
  void TriggerDictionaryInitialization_HPGeTreeEventDict_Impl() {
    static const char* headers[] = {
"HPGeTreeEvent.h",
0
    };
    static const char* includePaths[] = {
"/home/testov/root/include/",
"/home/testov/EliadeSorting/lib/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "HPGeTreeEventDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$HPGeTreeEvent.h")))  HPGeTreeEvent;
namespace std{template <typename _Tp> class __attribute__((annotate("$clingAutoload$bits/allocator.h")))  __attribute__((annotate("$clingAutoload$string")))  allocator;
}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "HPGeTreeEventDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "HPGeTreeEvent.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"HPGeTreeEvent", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("HPGeTreeEventDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_HPGeTreeEventDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_HPGeTreeEventDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_HPGeTreeEventDict() {
  TriggerDictionaryInitialization_HPGeTreeEventDict_Impl();
}
