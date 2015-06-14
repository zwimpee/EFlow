// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME srcdIdict

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "interface/JSON.h"
#include "interface/KFactorsVsTime.h"
#include "interface/HarnessMap.h"
#include "interface/RegionStability.h"
#include "interface/alcaPhiSym_tree_v3.h"
#include "interface/lumiIntervals.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static TClass *JSON_Dictionary();
   static void JSON_TClassManip(TClass*);
   static void delete_JSON(void *p);
   static void deleteArray_JSON(void *p);
   static void destruct_JSON(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::JSON*)
   {
      ::JSON *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::JSON));
      static ::ROOT::TGenericClassInfo 
         instance("JSON", "interface/JSON.h", 12,
                  typeid(::JSON), DefineBehavior(ptr, ptr),
                  &JSON_Dictionary, isa_proxy, 4,
                  sizeof(::JSON) );
      instance.SetDelete(&delete_JSON);
      instance.SetDeleteArray(&deleteArray_JSON);
      instance.SetDestructor(&destruct_JSON);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::JSON*)
   {
      return GenerateInitInstanceLocal((::JSON*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::JSON*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *JSON_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::JSON*)0x0)->GetClass();
      JSON_TClassManip(theClass);
   return theClass;
   }

   static void JSON_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *RegionStability_Dictionary();
   static void RegionStability_TClassManip(TClass*);
   static void *new_RegionStability(void *p = 0);
   static void *newArray_RegionStability(Long_t size, void *p);
   static void delete_RegionStability(void *p);
   static void deleteArray_RegionStability(void *p);
   static void destruct_RegionStability(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::RegionStability*)
   {
      ::RegionStability *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::RegionStability));
      static ::ROOT::TGenericClassInfo 
         instance("RegionStability", "interface/RegionStability.h", 8,
                  typeid(::RegionStability), DefineBehavior(ptr, ptr),
                  &RegionStability_Dictionary, isa_proxy, 4,
                  sizeof(::RegionStability) );
      instance.SetNew(&new_RegionStability);
      instance.SetNewArray(&newArray_RegionStability);
      instance.SetDelete(&delete_RegionStability);
      instance.SetDeleteArray(&deleteArray_RegionStability);
      instance.SetDestructor(&destruct_RegionStability);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::RegionStability*)
   {
      return GenerateInitInstanceLocal((::RegionStability*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::RegionStability*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *RegionStability_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::RegionStability*)0x0)->GetClass();
      RegionStability_TClassManip(theClass);
   return theClass;
   }

   static void RegionStability_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *KFactorsVsTime_Dictionary();
   static void KFactorsVsTime_TClassManip(TClass*);
   static void delete_KFactorsVsTime(void *p);
   static void deleteArray_KFactorsVsTime(void *p);
   static void destruct_KFactorsVsTime(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::KFactorsVsTime*)
   {
      ::KFactorsVsTime *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::KFactorsVsTime));
      static ::ROOT::TGenericClassInfo 
         instance("KFactorsVsTime", "interface/KFactorsVsTime.h", 7,
                  typeid(::KFactorsVsTime), DefineBehavior(ptr, ptr),
                  &KFactorsVsTime_Dictionary, isa_proxy, 4,
                  sizeof(::KFactorsVsTime) );
      instance.SetDelete(&delete_KFactorsVsTime);
      instance.SetDeleteArray(&deleteArray_KFactorsVsTime);
      instance.SetDestructor(&destruct_KFactorsVsTime);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::KFactorsVsTime*)
   {
      return GenerateInitInstanceLocal((::KFactorsVsTime*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::KFactorsVsTime*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *KFactorsVsTime_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::KFactorsVsTime*)0x0)->GetClass();
      KFactorsVsTime_TClassManip(theClass);
   return theClass;
   }

   static void KFactorsVsTime_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *lumiIntervals_Dictionary();
   static void lumiIntervals_TClassManip(TClass*);
   static void delete_lumiIntervals(void *p);
   static void deleteArray_lumiIntervals(void *p);
   static void destruct_lumiIntervals(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::lumiIntervals*)
   {
      ::lumiIntervals *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::lumiIntervals));
      static ::ROOT::TGenericClassInfo 
         instance("lumiIntervals", "interface/lumiIntervals.h", 6,
                  typeid(::lumiIntervals), DefineBehavior(ptr, ptr),
                  &lumiIntervals_Dictionary, isa_proxy, 4,
                  sizeof(::lumiIntervals) );
      instance.SetDelete(&delete_lumiIntervals);
      instance.SetDeleteArray(&deleteArray_lumiIntervals);
      instance.SetDestructor(&destruct_lumiIntervals);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::lumiIntervals*)
   {
      return GenerateInitInstanceLocal((::lumiIntervals*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::lumiIntervals*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *lumiIntervals_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::lumiIntervals*)0x0)->GetClass();
      lumiIntervals_TClassManip(theClass);
   return theClass;
   }

   static void lumiIntervals_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *alcaPhiSym_tree_v3_Dictionary();
   static void alcaPhiSym_tree_v3_TClassManip(TClass*);
   static void *new_alcaPhiSym_tree_v3(void *p = 0);
   static void *newArray_alcaPhiSym_tree_v3(Long_t size, void *p);
   static void delete_alcaPhiSym_tree_v3(void *p);
   static void deleteArray_alcaPhiSym_tree_v3(void *p);
   static void destruct_alcaPhiSym_tree_v3(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::alcaPhiSym_tree_v3*)
   {
      ::alcaPhiSym_tree_v3 *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::alcaPhiSym_tree_v3));
      static ::ROOT::TGenericClassInfo 
         instance("alcaPhiSym_tree_v3", "interface/alcaPhiSym_tree_v3.h", 17,
                  typeid(::alcaPhiSym_tree_v3), DefineBehavior(ptr, ptr),
                  &alcaPhiSym_tree_v3_Dictionary, isa_proxy, 4,
                  sizeof(::alcaPhiSym_tree_v3) );
      instance.SetNew(&new_alcaPhiSym_tree_v3);
      instance.SetNewArray(&newArray_alcaPhiSym_tree_v3);
      instance.SetDelete(&delete_alcaPhiSym_tree_v3);
      instance.SetDeleteArray(&deleteArray_alcaPhiSym_tree_v3);
      instance.SetDestructor(&destruct_alcaPhiSym_tree_v3);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::alcaPhiSym_tree_v3*)
   {
      return GenerateInitInstanceLocal((::alcaPhiSym_tree_v3*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::alcaPhiSym_tree_v3*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *alcaPhiSym_tree_v3_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::alcaPhiSym_tree_v3*)0x0)->GetClass();
      alcaPhiSym_tree_v3_TClassManip(theClass);
   return theClass;
   }

   static void alcaPhiSym_tree_v3_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrapper around operator delete
   static void delete_JSON(void *p) {
      delete ((::JSON*)p);
   }
   static void deleteArray_JSON(void *p) {
      delete [] ((::JSON*)p);
   }
   static void destruct_JSON(void *p) {
      typedef ::JSON current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::JSON

namespace ROOT {
   // Wrappers around operator new
   static void *new_RegionStability(void *p) {
      return  p ? new(p) ::RegionStability : new ::RegionStability;
   }
   static void *newArray_RegionStability(Long_t nElements, void *p) {
      return p ? new(p) ::RegionStability[nElements] : new ::RegionStability[nElements];
   }
   // Wrapper around operator delete
   static void delete_RegionStability(void *p) {
      delete ((::RegionStability*)p);
   }
   static void deleteArray_RegionStability(void *p) {
      delete [] ((::RegionStability*)p);
   }
   static void destruct_RegionStability(void *p) {
      typedef ::RegionStability current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::RegionStability

namespace ROOT {
   // Wrapper around operator delete
   static void delete_KFactorsVsTime(void *p) {
      delete ((::KFactorsVsTime*)p);
   }
   static void deleteArray_KFactorsVsTime(void *p) {
      delete [] ((::KFactorsVsTime*)p);
   }
   static void destruct_KFactorsVsTime(void *p) {
      typedef ::KFactorsVsTime current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::KFactorsVsTime

namespace ROOT {
   // Wrapper around operator delete
   static void delete_lumiIntervals(void *p) {
      delete ((::lumiIntervals*)p);
   }
   static void deleteArray_lumiIntervals(void *p) {
      delete [] ((::lumiIntervals*)p);
   }
   static void destruct_lumiIntervals(void *p) {
      typedef ::lumiIntervals current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::lumiIntervals

namespace ROOT {
   // Wrappers around operator new
   static void *new_alcaPhiSym_tree_v3(void *p) {
      return  p ? new(p) ::alcaPhiSym_tree_v3 : new ::alcaPhiSym_tree_v3;
   }
   static void *newArray_alcaPhiSym_tree_v3(Long_t nElements, void *p) {
      return p ? new(p) ::alcaPhiSym_tree_v3[nElements] : new ::alcaPhiSym_tree_v3[nElements];
   }
   // Wrapper around operator delete
   static void delete_alcaPhiSym_tree_v3(void *p) {
      delete ((::alcaPhiSym_tree_v3*)p);
   }
   static void deleteArray_alcaPhiSym_tree_v3(void *p) {
      delete [] ((::alcaPhiSym_tree_v3*)p);
   }
   static void destruct_alcaPhiSym_tree_v3(void *p) {
      typedef ::alcaPhiSym_tree_v3 current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::alcaPhiSym_tree_v3

namespace ROOT {
   static TClass *vectorlEstringgR_Dictionary();
   static void vectorlEstringgR_TClassManip(TClass*);
   static void *new_vectorlEstringgR(void *p = 0);
   static void *newArray_vectorlEstringgR(Long_t size, void *p);
   static void delete_vectorlEstringgR(void *p);
   static void deleteArray_vectorlEstringgR(void *p);
   static void destruct_vectorlEstringgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<string>*)
   {
      vector<string> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<string>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<string>", -2, "vector", 214,
                  typeid(vector<string>), DefineBehavior(ptr, ptr),
                  &vectorlEstringgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<string>) );
      instance.SetNew(&new_vectorlEstringgR);
      instance.SetNewArray(&newArray_vectorlEstringgR);
      instance.SetDelete(&delete_vectorlEstringgR);
      instance.SetDeleteArray(&deleteArray_vectorlEstringgR);
      instance.SetDestructor(&destruct_vectorlEstringgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<string> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<string>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEstringgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<string>*)0x0)->GetClass();
      vectorlEstringgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEstringgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEstringgR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<string> : new vector<string>;
   }
   static void *newArray_vectorlEstringgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<string>[nElements] : new vector<string>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEstringgR(void *p) {
      delete ((vector<string>*)p);
   }
   static void deleteArray_vectorlEstringgR(void *p) {
      delete [] ((vector<string>*)p);
   }
   static void destruct_vectorlEstringgR(void *p) {
      typedef vector<string> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<string>

namespace ROOT {
   static TClass *vectorlEpairlElumiIntervalscLcLrunLScOlumiIntervalscLcLrunLSgRsPgR_Dictionary();
   static void vectorlEpairlElumiIntervalscLcLrunLScOlumiIntervalscLcLrunLSgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEpairlElumiIntervalscLcLrunLScOlumiIntervalscLcLrunLSgRsPgR(void *p = 0);
   static void *newArray_vectorlEpairlElumiIntervalscLcLrunLScOlumiIntervalscLcLrunLSgRsPgR(Long_t size, void *p);
   static void delete_vectorlEpairlElumiIntervalscLcLrunLScOlumiIntervalscLcLrunLSgRsPgR(void *p);
   static void deleteArray_vectorlEpairlElumiIntervalscLcLrunLScOlumiIntervalscLcLrunLSgRsPgR(void *p);
   static void destruct_vectorlEpairlElumiIntervalscLcLrunLScOlumiIntervalscLcLrunLSgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<pair<lumiIntervals::runLS,lumiIntervals::runLS> >*)
   {
      vector<pair<lumiIntervals::runLS,lumiIntervals::runLS> > *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<pair<lumiIntervals::runLS,lumiIntervals::runLS> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<pair<lumiIntervals::runLS,lumiIntervals::runLS> >", -2, "vector", 214,
                  typeid(vector<pair<lumiIntervals::runLS,lumiIntervals::runLS> >), DefineBehavior(ptr, ptr),
                  &vectorlEpairlElumiIntervalscLcLrunLScOlumiIntervalscLcLrunLSgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<pair<lumiIntervals::runLS,lumiIntervals::runLS> >) );
      instance.SetNew(&new_vectorlEpairlElumiIntervalscLcLrunLScOlumiIntervalscLcLrunLSgRsPgR);
      instance.SetNewArray(&newArray_vectorlEpairlElumiIntervalscLcLrunLScOlumiIntervalscLcLrunLSgRsPgR);
      instance.SetDelete(&delete_vectorlEpairlElumiIntervalscLcLrunLScOlumiIntervalscLcLrunLSgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEpairlElumiIntervalscLcLrunLScOlumiIntervalscLcLrunLSgRsPgR);
      instance.SetDestructor(&destruct_vectorlEpairlElumiIntervalscLcLrunLScOlumiIntervalscLcLrunLSgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<pair<lumiIntervals::runLS,lumiIntervals::runLS> > >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<pair<lumiIntervals::runLS,lumiIntervals::runLS> >*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEpairlElumiIntervalscLcLrunLScOlumiIntervalscLcLrunLSgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<pair<lumiIntervals::runLS,lumiIntervals::runLS> >*)0x0)->GetClass();
      vectorlEpairlElumiIntervalscLcLrunLScOlumiIntervalscLcLrunLSgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEpairlElumiIntervalscLcLrunLScOlumiIntervalscLcLrunLSgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEpairlElumiIntervalscLcLrunLScOlumiIntervalscLcLrunLSgRsPgR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<pair<lumiIntervals::runLS,lumiIntervals::runLS> > : new vector<pair<lumiIntervals::runLS,lumiIntervals::runLS> >;
   }
   static void *newArray_vectorlEpairlElumiIntervalscLcLrunLScOlumiIntervalscLcLrunLSgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<pair<lumiIntervals::runLS,lumiIntervals::runLS> >[nElements] : new vector<pair<lumiIntervals::runLS,lumiIntervals::runLS> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEpairlElumiIntervalscLcLrunLScOlumiIntervalscLcLrunLSgRsPgR(void *p) {
      delete ((vector<pair<lumiIntervals::runLS,lumiIntervals::runLS> >*)p);
   }
   static void deleteArray_vectorlEpairlElumiIntervalscLcLrunLScOlumiIntervalscLcLrunLSgRsPgR(void *p) {
      delete [] ((vector<pair<lumiIntervals::runLS,lumiIntervals::runLS> >*)p);
   }
   static void destruct_vectorlEpairlElumiIntervalscLcLrunLScOlumiIntervalscLcLrunLSgRsPgR(void *p) {
      typedef vector<pair<lumiIntervals::runLS,lumiIntervals::runLS> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<pair<lumiIntervals::runLS,lumiIntervals::runLS> >

namespace ROOT {
   static TClass *vectorlEpairlEintcOintgRsPgR_Dictionary();
   static void vectorlEpairlEintcOintgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEpairlEintcOintgRsPgR(void *p = 0);
   static void *newArray_vectorlEpairlEintcOintgRsPgR(Long_t size, void *p);
   static void delete_vectorlEpairlEintcOintgRsPgR(void *p);
   static void deleteArray_vectorlEpairlEintcOintgRsPgR(void *p);
   static void destruct_vectorlEpairlEintcOintgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<pair<int,int> >*)
   {
      vector<pair<int,int> > *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<pair<int,int> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<pair<int,int> >", -2, "vector", 214,
                  typeid(vector<pair<int,int> >), DefineBehavior(ptr, ptr),
                  &vectorlEpairlEintcOintgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<pair<int,int> >) );
      instance.SetNew(&new_vectorlEpairlEintcOintgRsPgR);
      instance.SetNewArray(&newArray_vectorlEpairlEintcOintgRsPgR);
      instance.SetDelete(&delete_vectorlEpairlEintcOintgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEpairlEintcOintgRsPgR);
      instance.SetDestructor(&destruct_vectorlEpairlEintcOintgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<pair<int,int> > >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<pair<int,int> >*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEpairlEintcOintgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<pair<int,int> >*)0x0)->GetClass();
      vectorlEpairlEintcOintgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEpairlEintcOintgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEpairlEintcOintgRsPgR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<pair<int,int> > : new vector<pair<int,int> >;
   }
   static void *newArray_vectorlEpairlEintcOintgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<pair<int,int> >[nElements] : new vector<pair<int,int> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEpairlEintcOintgRsPgR(void *p) {
      delete ((vector<pair<int,int> >*)p);
   }
   static void deleteArray_vectorlEpairlEintcOintgRsPgR(void *p) {
      delete [] ((vector<pair<int,int> >*)p);
   }
   static void destruct_vectorlEpairlEintcOintgRsPgR(void *p) {
      typedef vector<pair<int,int> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<pair<int,int> >

namespace ROOT {
   static TClass *vectorlEintgR_Dictionary();
   static void vectorlEintgR_TClassManip(TClass*);
   static void *new_vectorlEintgR(void *p = 0);
   static void *newArray_vectorlEintgR(Long_t size, void *p);
   static void delete_vectorlEintgR(void *p);
   static void deleteArray_vectorlEintgR(void *p);
   static void destruct_vectorlEintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<int>*)
   {
      vector<int> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<int>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<int>", -2, "vector", 214,
                  typeid(vector<int>), DefineBehavior(ptr, ptr),
                  &vectorlEintgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<int>) );
      instance.SetNew(&new_vectorlEintgR);
      instance.SetNewArray(&newArray_vectorlEintgR);
      instance.SetDelete(&delete_vectorlEintgR);
      instance.SetDeleteArray(&deleteArray_vectorlEintgR);
      instance.SetDestructor(&destruct_vectorlEintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<int> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<int>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<int>*)0x0)->GetClass();
      vectorlEintgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEintgR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<int> : new vector<int>;
   }
   static void *newArray_vectorlEintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<int>[nElements] : new vector<int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEintgR(void *p) {
      delete ((vector<int>*)p);
   }
   static void deleteArray_vectorlEintgR(void *p) {
      delete [] ((vector<int>*)p);
   }
   static void destruct_vectorlEintgR(void *p) {
      typedef vector<int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<int>

namespace ROOT {
   static TClass *vectorlEfloatgR_Dictionary();
   static void vectorlEfloatgR_TClassManip(TClass*);
   static void *new_vectorlEfloatgR(void *p = 0);
   static void *newArray_vectorlEfloatgR(Long_t size, void *p);
   static void delete_vectorlEfloatgR(void *p);
   static void deleteArray_vectorlEfloatgR(void *p);
   static void destruct_vectorlEfloatgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<float>*)
   {
      vector<float> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<float>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<float>", -2, "vector", 214,
                  typeid(vector<float>), DefineBehavior(ptr, ptr),
                  &vectorlEfloatgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<float>) );
      instance.SetNew(&new_vectorlEfloatgR);
      instance.SetNewArray(&newArray_vectorlEfloatgR);
      instance.SetDelete(&delete_vectorlEfloatgR);
      instance.SetDeleteArray(&deleteArray_vectorlEfloatgR);
      instance.SetDestructor(&destruct_vectorlEfloatgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<float> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<float>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEfloatgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<float>*)0x0)->GetClass();
      vectorlEfloatgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEfloatgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEfloatgR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<float> : new vector<float>;
   }
   static void *newArray_vectorlEfloatgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<float>[nElements] : new vector<float>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEfloatgR(void *p) {
      delete ((vector<float>*)p);
   }
   static void deleteArray_vectorlEfloatgR(void *p) {
      delete [] ((vector<float>*)p);
   }
   static void destruct_vectorlEfloatgR(void *p) {
      typedef vector<float> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<float>

namespace ROOT {
   static TClass *vectorlEboolgR_Dictionary();
   static void vectorlEboolgR_TClassManip(TClass*);
   static void *new_vectorlEboolgR(void *p = 0);
   static void *newArray_vectorlEboolgR(Long_t size, void *p);
   static void delete_vectorlEboolgR(void *p);
   static void deleteArray_vectorlEboolgR(void *p);
   static void destruct_vectorlEboolgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<bool>*)
   {
      vector<bool> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<bool>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<bool>", -2, "vector", 526,
                  typeid(vector<bool>), DefineBehavior(ptr, ptr),
                  &vectorlEboolgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<bool>) );
      instance.SetNew(&new_vectorlEboolgR);
      instance.SetNewArray(&newArray_vectorlEboolgR);
      instance.SetDelete(&delete_vectorlEboolgR);
      instance.SetDeleteArray(&deleteArray_vectorlEboolgR);
      instance.SetDestructor(&destruct_vectorlEboolgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<bool> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<bool>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEboolgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<bool>*)0x0)->GetClass();
      vectorlEboolgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEboolgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEboolgR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<bool> : new vector<bool>;
   }
   static void *newArray_vectorlEboolgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<bool>[nElements] : new vector<bool>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEboolgR(void *p) {
      delete ((vector<bool>*)p);
   }
   static void deleteArray_vectorlEboolgR(void *p) {
      delete [] ((vector<bool>*)p);
   }
   static void destruct_vectorlEboolgR(void *p) {
      typedef vector<bool> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<bool>

namespace ROOT {
   static TClass *maplEpairlEintcOintgRcOfloatgR_Dictionary();
   static void maplEpairlEintcOintgRcOfloatgR_TClassManip(TClass*);
   static void *new_maplEpairlEintcOintgRcOfloatgR(void *p = 0);
   static void *newArray_maplEpairlEintcOintgRcOfloatgR(Long_t size, void *p);
   static void delete_maplEpairlEintcOintgRcOfloatgR(void *p);
   static void deleteArray_maplEpairlEintcOintgRcOfloatgR(void *p);
   static void destruct_maplEpairlEintcOintgRcOfloatgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<pair<int,int>,float>*)
   {
      map<pair<int,int>,float> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<pair<int,int>,float>));
      static ::ROOT::TGenericClassInfo 
         instance("map<pair<int,int>,float>", -2, "map", 96,
                  typeid(map<pair<int,int>,float>), DefineBehavior(ptr, ptr),
                  &maplEpairlEintcOintgRcOfloatgR_Dictionary, isa_proxy, 0,
                  sizeof(map<pair<int,int>,float>) );
      instance.SetNew(&new_maplEpairlEintcOintgRcOfloatgR);
      instance.SetNewArray(&newArray_maplEpairlEintcOintgRcOfloatgR);
      instance.SetDelete(&delete_maplEpairlEintcOintgRcOfloatgR);
      instance.SetDeleteArray(&deleteArray_maplEpairlEintcOintgRcOfloatgR);
      instance.SetDestructor(&destruct_maplEpairlEintcOintgRcOfloatgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<pair<int,int>,float> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const map<pair<int,int>,float>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEpairlEintcOintgRcOfloatgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<pair<int,int>,float>*)0x0)->GetClass();
      maplEpairlEintcOintgRcOfloatgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEpairlEintcOintgRcOfloatgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEpairlEintcOintgRcOfloatgR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) map<pair<int,int>,float> : new map<pair<int,int>,float>;
   }
   static void *newArray_maplEpairlEintcOintgRcOfloatgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) map<pair<int,int>,float>[nElements] : new map<pair<int,int>,float>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEpairlEintcOintgRcOfloatgR(void *p) {
      delete ((map<pair<int,int>,float>*)p);
   }
   static void deleteArray_maplEpairlEintcOintgRcOfloatgR(void *p) {
      delete [] ((map<pair<int,int>,float>*)p);
   }
   static void destruct_maplEpairlEintcOintgRcOfloatgR(void *p) {
      typedef map<pair<int,int>,float> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<pair<int,int>,float>

namespace ROOT {
   static TClass *maplEintcOvectorlEpairlEintcOintgRsPgRsPgR_Dictionary();
   static void maplEintcOvectorlEpairlEintcOintgRsPgRsPgR_TClassManip(TClass*);
   static void *new_maplEintcOvectorlEpairlEintcOintgRsPgRsPgR(void *p = 0);
   static void *newArray_maplEintcOvectorlEpairlEintcOintgRsPgRsPgR(Long_t size, void *p);
   static void delete_maplEintcOvectorlEpairlEintcOintgRsPgRsPgR(void *p);
   static void deleteArray_maplEintcOvectorlEpairlEintcOintgRsPgRsPgR(void *p);
   static void destruct_maplEintcOvectorlEpairlEintcOintgRsPgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,vector<pair<int,int> > >*)
   {
      map<int,vector<pair<int,int> > > *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,vector<pair<int,int> > >));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,vector<pair<int,int> > >", -2, "map", 96,
                  typeid(map<int,vector<pair<int,int> > >), DefineBehavior(ptr, ptr),
                  &maplEintcOvectorlEpairlEintcOintgRsPgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,vector<pair<int,int> > >) );
      instance.SetNew(&new_maplEintcOvectorlEpairlEintcOintgRsPgRsPgR);
      instance.SetNewArray(&newArray_maplEintcOvectorlEpairlEintcOintgRsPgRsPgR);
      instance.SetDelete(&delete_maplEintcOvectorlEpairlEintcOintgRsPgRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOvectorlEpairlEintcOintgRsPgRsPgR);
      instance.SetDestructor(&destruct_maplEintcOvectorlEpairlEintcOintgRsPgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,vector<pair<int,int> > > >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const map<int,vector<pair<int,int> > >*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOvectorlEpairlEintcOintgRsPgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,vector<pair<int,int> > >*)0x0)->GetClass();
      maplEintcOvectorlEpairlEintcOintgRsPgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOvectorlEpairlEintcOintgRsPgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOvectorlEpairlEintcOintgRsPgRsPgR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) map<int,vector<pair<int,int> > > : new map<int,vector<pair<int,int> > >;
   }
   static void *newArray_maplEintcOvectorlEpairlEintcOintgRsPgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) map<int,vector<pair<int,int> > >[nElements] : new map<int,vector<pair<int,int> > >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOvectorlEpairlEintcOintgRsPgRsPgR(void *p) {
      delete ((map<int,vector<pair<int,int> > >*)p);
   }
   static void deleteArray_maplEintcOvectorlEpairlEintcOintgRsPgRsPgR(void *p) {
      delete [] ((map<int,vector<pair<int,int> > >*)p);
   }
   static void destruct_maplEintcOvectorlEpairlEintcOintgRsPgRsPgR(void *p) {
      typedef map<int,vector<pair<int,int> > > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,vector<pair<int,int> > >

namespace {
  void TriggerDictionaryInitialization_dict_Impl() {
    static const char* headers[] = {
"interface/JSON.h",
"interface/KFactorsVsTime.h",
"interface/HarnessMap.h",
"interface/RegionStability.h",
"interface/alcaPhiSym_tree_v3.h",
"interface/lumiIntervals.h",
0
    };
    static const char* includePaths[] = {
"/cvmfs/cms.cern.ch/slc6_amd64_gcc491/lcg/root/6.02.00-odfocd4/include",
"/cvmfs/cms.cern.ch/slc6_amd64_gcc491/lcg/root/6.02.00-odfocd4/include",
"/afs/cern.ch/work/m/meridian/CMSSW744p2/src/EFlow/EcalEFlowAnalysis/",
0
    };
    static const char* fwdDeclCode = 
R"DICTFWDDCLS(
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$interface/JSON.h")))  JSON;
class __attribute__((annotate("$clingAutoload$interface/RegionStability.h")))  RegionStability;
class __attribute__((annotate("$clingAutoload$interface/KFactorsVsTime.h")))  KFactorsVsTime;
class __attribute__((annotate("$clingAutoload$interface/lumiIntervals.h")))  lumiIntervals;
class __attribute__((annotate("$clingAutoload$interface/alcaPhiSym_tree_v3.h")))  alcaPhiSym_tree_v3;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "interface/JSON.h"
#include "interface/KFactorsVsTime.h"
#include "interface/HarnessMap.h"
#include "interface/RegionStability.h"
#include "interface/alcaPhiSym_tree_v3.h"
#include "interface/lumiIntervals.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"JSON", payloadCode, "@",
"KFactorsVsTime", payloadCode, "@",
"RegionStability", payloadCode, "@",
"alcaPhiSym_tree_v3", payloadCode, "@",
"lumiIntervals", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("dict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_dict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_dict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_dict() {
  TriggerDictionaryInitialization_dict_Impl();
}
