#pragma once
class Function
{
public:
	template <typename Func, typename... Args>
	Function(Func func, Args&& ... args)
	{
		m_func = std::bind(func, std::forward<Args>(args)...);
	}

	void execute()
	{
		return m_func();
	}
private:
	std::function<void()> m_func;
};