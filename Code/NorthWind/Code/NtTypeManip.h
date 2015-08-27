
#pragma once

#include <cassert>

namespace nt {

// Test T is Pointer Type
template <typename T>
struct NtIsPtr
{
private:
	template <typename V>
	struct Nest
	{
		enum { Result = FALSE, };
	};

	template <typename V>
	struct Nest<V*>
	{
		enum { Result = TRUE, };
	};

public:
	enum { Result = Nest<T>::Result, };
};

struct Safe_Delete
{
	template <typename T>
	void operator() (T object)
	{
		assert(NtIsPtr<T>::Result == TRUE);

		if (object)
		{
			delete object;
			object = nullptr;
		}
	}
};

struct Safe_Delete_Array
{
	template <typename T>
	void operator() (T object)
	{
		assert(NtIsPtr<T>::Result == TRUE);

		if (object)
		{
			delete [] object;
			object = nullptr;
		}
	}
};

struct Safe_Delete_Second
{
	template <typename T>
	void operator() (T object)
	{
		assert(NtIsPtr<T::second_type>::Result == TRUE);

		delete &object.second;
		object.second = nullptr;
	}
};

struct Safe_Release
{
	template <typename T>
	void operator() (T object)
	{
		assert(NtIsPtr<T>::Result == TRUE);

		if (object)
		{
			object->Release();
		}
	}
};

#define SAFE_DELETE(ob) Safe_Delete()(ob)
#define SAFE_DELETE_ARRAY(ob) Safe_Delete_Array()(ob)
#define SAFE_RELEASE(ob) Safe_Release()(ob)


template <typename T>
class NtScopedObject
{
public:
	explicit NtScopedObject(T* obj = nullptr) : m_obj(obj)
	{

	}
	~NtScopedObject()
	{
		SAFE_RELEASE(m_obj);
	}

	T& operator*() { return *m_obj; }
	T* operator->() { return m_obj; }
	T** operator&() { return &m_obj; }

	void Reset(T* obj)
	{
		SAFE_RELEASE(obj);
		m_obj = obj;
	}

	T* Get() const
	{
		return m_obj;
	}


private:
	NtScopedObject(const NtScopedObject& obj);
	NtScopedObject& operator=(const NtScopedObject& obj);

	T* m_obj;
};


// Test T is Reference Type
template <typename T>
struct NtIsRef
{
private:
	template <typename V>
	struct Nest
	{
		enum { Result = FALSE, };
	};

	template <typename V>
	struct Nest<V&>
	{
		enum { Result = TRUE, };
	};

public:
	enum { Result = Nest<T>::Result, };
};

// MemFun
template <typename type_, typename result_, typename arg_>
struct NtMemFun
{
	explicit NtMemFun(type_& inst, result_ (type_::*memfunc)(arg_))
		: inst_(inst)
		, _memfunc(memfunc)
	{
	}

	result_ operator()(arg_ arg)
	{
		return (inst_->*_memfunc)(arg);
	}

	type_*	inst_;
	result_ (type_::*_memfunc)(arg_);
};

template <typename type_, typename result_, typename arg_>
struct NtConstMemFun
{
	explicit NtConstMemFun(const type_& inst, result_ (type_::*memfunc)(arg_) const)
		: inst_(inst)
		, _memfunc(memfunc)
	{

	}

	result_ operator()(arg_ arg) const
	{
		return (inst_->*_memfunc)(arg);
	}

	const type_* inst_;
	result_ (type_::*_memfunc)(arg_) const;
};


template <typename type_, typename result_, typename arg_>
NtMemFun<type_, result_, arg_> MemFunc(type_& inst, result_ (type_::*memfunc)(arg_))
{
	return NtMemFun<type_, result_, arg_>(inst, memfunc);
}

template <typename type_, typename result_, typename arg_>
NtConstMemFun<type_, result_, arg_> MemFunc(const type_& inst, result_ (type_::*memfunc)(arg_) const)
{
	return NtConstMemFun<type_, result_, arg_>(inst, memfunc);
}


// For Functor

// null type class
class NtNullType
{

};


// for destruct object
class NtDestroyObject
{
public:
	~NtDestroyObject()
	{

	}
};


// 
template <typename RETURN,	// return Type
		typename T1,		// first parameter
		typename T2>		// second parameter
class NtIFunctor;


// for only use return type, not use parameter
template <typename RETURN>
class NtIFunctor<RETURN,	// return type
			NtNullType,		// not use 
			NtNullType>		// not use
			: public NtDestroyObject
{
public:
	virtual RETURN operator()() = 0;
};

// for use return type, only use first parameter
template <typename RETURN, typename T1>
class NtIFunctor<RETURN,	// return type
				T1,			// use first parameter
				NtNullType>	// not use
				: public NtDestroyObject
{
public:
	virtual RETURN operator()(T1 param1) = 0;
};


// return type + All use first and second parameter
template <typename RETURN, typename T1, typename T2>
class NtIFunctor : public NtDestroyObject
{
public:
	virtual RETURN operator()(T1 param1, T2 param2) = 0;
};


// 
template <typename FUNCTOR_CORE, typename FUNC>
class NtFunctorBase : public NtIFunctor<
	typename FUNCTOR_CORE::return_type,
	typename FUNCTOR_CORE::param_type1,
	typename FUNCTOR_CORE::param_type2>
{
	typedef typename FUNCTOR_CORE::return_type return_type;
	typedef typename FUNCTOR_CORE::param_type1 param_type1;
	typedef typename FUNCTOR_CORE::param_type2 param_type2;

public:
	NtFunctorBase(FUNC fn)
		: m_fn(fn)
	{

	}

	return_type operator()()
	{
		return (*m_fn)();
	}

	return_type operator()(param_type1 param1)
	{
		return (*m_fn)(param1);
	}

	return_type operator()(param_type1 param1, param_type2 param2)
	{
		return (*m_fn)(param1, param2);
	}

private:
	FUNC m_fn;
};

template <typename FUNCTOR_CORE, typename OBJECT, typename FUNC>
class NtFunctorBase_ob : public NtIFunctor<
	typename FUNCTOR_CORE::return_type,
	typename FUNCTOR_CORE::param_type1,
	typename FUNCTOR_CORE::param_type2>
{
	typedef typename FUNCTOR_CORE::return_type return_type;
	typedef typename FUNCTOR_CORE::param_type1 param_type1;
	typedef typename FUNCTOR_CORE::param_type2 param_type2;

public:
	NtFunctorBase_ob(const OBJECT& ob, FUNC fn)
		: m_object(ob)
		, m_fn(fn)
	{

	}

	return_type operator()()
	{
		return ((*m_object).*m_fn)();
	}

	return_type operator()(param_type1 param1)
	{
		return ((*m_object).*m_fn)(param1);
	}

	return_type operator()(param_type1 param1, param_type2 param2)
	{
		return ((*m_object).*m_fn)(param1, param2);
	}

private:
	OBJECT m_object;
	FUNC m_fn;
};


// 
template<typename RETURN, typename T1 = NtNullType, typename T2 = NtNullType>
class NtFunctor
{
public:
	typedef RETURN return_type;
	typedef T1 param_type1;
	typedef T2 param_type2;

	NtFunctor()
		: m_impl(nullptr)
	{
		Destory();
	}

	~NtFunctor()
	{
		
	}

	template <typename FUNC>
	NtFunctor(FUNC fn)
	{
		Assign(fn);
	}

	template <typename OBJECT, typename FUNC>
	NtFunctor(const OBJECT& ob, FUNC fn)
	{
		Assign(ob, fn);
	}

	template <typename FUNC>
	NtFunctor& operator=(FUNC fn)
	{
		Destory();
		Assign(fn);

		return *this;
	}

	RETURN operator()()
	{
		return (*m_impl)();
	}

	RETURN operator()(T1 param1)
	{
		return (*m_impl)(param1);
	}

	RETURN operator()(T1 param1, T2 param2)
	{
		return (*m_impl)(param1, param2);
	}

private:
	void Destory()
	{
		if (nullptr == m_impl)
		{
			return;
		}

		SAFE_DELETE(m_impl);
	}

	template <typename FUNC>
	void Assign(FUNC fn)
	{
		m_impl = new NtFunctorBase<NtFunctor, FUNC>(fn);
	}

	template <typename OBJECT, typename FUNC>
	void Assign(const OBJECT& ob, FUNC fn)
	{
		m_impl = new NtFunctorBase_ob<NtFunctor, OBJECT, FUNC>(ob, fn);
	}


private:
	NtIFunctor<RETURN, T1, T2>* m_impl;
};


class none_type
{

};

class destro
{
public:
	virtual ~destro()
	{

	}
};


template <typename RETURN, typename PARAM1, typename PARAM2>
class IFunctor;

template <typename RETURN>
class IFunctor<RETURN, none_type, none_type> : public destro
{
public:
	virtual RETURN operator()() = 0;
};


template <typename RETURN, typename PARAM1>
class IFunctor<RETURN, PARAM1, none_type> : public destro
{
public:
	virtual RETURN operator()(PARAM1 param1) = 0;
};


template <typename RETURN, typename PARAM1, typename PARAM2>
class IFunctor : public destro
{
public:
	virtual RETURN operator()(PARAM1 param1, PARAM2 param2) = 0;
};


template <typename PRT, typename FUNC>
class IFunctorSub : public IFunctor<typename PRT::return_type, typename PRT::param_type1, typename PRT::param_type2>
{
public:
	typedef typename PRT::return_type return_type;
	typedef typename PRT::param_type1 param_type1;
	typedef typename PRT::param_type2 param_type2;

	IFunctorSub(FUNC func)
		: m_func(func)
	{

	}


	~IFunctorSub() {}

	return_type operator()()
	{
		return (*m_func)();
	}

	return_type operator()(param_type1 param1)
	{
		return (*m_func)(param1);
	}

	return_type operator()(param_type1 param1, param_type2 param2)
	{
		return (*m_func)(param1, param2);
	}

private:
	FUNC m_func;
};

template <typename PRT, typename OBJECT, typename FUNC>
class IFunctorObjSub : public IFunctor<typename PRT::return_type, typename PRT::param_type1, typename PRT::param_type2>
{
public:
	typedef typename PRT::return_type return_type;
	typedef typename PRT::param_type1 param_type1;
	typedef typename PRT::param_type2 param_type2;

	IFunctorObjSub(OBJECT obj, FUNC func)
		: m_obj(obj)
		, m_func(func)
	{

	}

	return_type operator()()
	{
		return ((*m_obj).*m_func)();
	}

	return_type operator()(param_type1 param1)
	{
		return ((*m_obj).*m_func)(param1);
	}

	return_type operator()(param_type1 param1, param_type2 param2)
	{
		return ((*m_obj).*m_func)(param1, param2);
	}

private:
	OBJECT m_obj;
	FUNC m_func;
};

template <typename RETURN, typename PARAM1 = none_type, typename PARAM2 = none_type>
class NFunctor
{
public:
	typedef RETURN return_type;
	typedef PARAM1 param_type1;
	typedef PARAM2 param_type2;

	template <typename FUNC>
	NFunctor(FUNC func)
		: m_impl(nullptr)
	{
		m_impl = new IFunctorSub<NFunctor, FUNC>(func);
	}

	template <typename OBJECT, typename FUNC>
	NFunctor(OBJECT obj, FUNC func)
		: m_impl(nullptr)
	{
		m_impl = new IFunctorObjSub<NFunctor, OBJECT, FUNC>(obj, func);
	}

	~NFunctor()
	{
		SAFE_DELETE(m_impl);
	}

	return_type operator()()
	{
		return (*m_impl)();
	}

	return_type operator()(param_type1 param1)
	{
		return (*m_impl)(param1);
	}

	return_type operator()(param_type1 param1, param_type2 param2)
	{
		return (*m_impl)(param1, param2);
	}


private:
	IFunctor<RETURN, PARAM1, PARAM2>* m_impl;
};

class NtFlag
{
public:
	NtFlag() : m_flags(0) {}
	~NtFlag() {}

	void TurnOn(ntInt flag)		{ m_flags |= flag; }
	void TurnOff(ntInt flag)	{ m_flags &= ~flag; }

	bool IsEnable(ntInt flag) const	{ return (m_flags & flag) != 0 ? true : false; }
	void Set(ntInt flag)		{ m_flags = flag; }

private:
	ntInt m_flags;
};

}	// namespace nt

