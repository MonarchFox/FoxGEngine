#pragma once

#include <queue>
#include <bitset>


class FxKeyboard
{
	friend class FxWindow;
public:

	class FxEvent
	{
	public:
		enum class KeyType: uint32_t
		{
			PRESS,
			RELEASE,
			INVALID
		};

	private:
		KeyType m_KeyType;
		unsigned char m_KeyCode;

	public:
		FxEvent() : m_KeyType(KeyType::INVALID), m_KeyCode(0u){}
		FxEvent(KeyType type, unsigned char code) noexcept
			: m_KeyType(type), m_KeyCode(code) {}

		bool IsPress() const noexcept  { return m_KeyType == KeyType::PRESS;	}
		bool IsRelase() const noexcept { return m_KeyType == KeyType::RELEASE;	}
		bool IsValid() const noexcept  { return m_KeyType == KeyType::INVALID;	}

		unsigned char GetCode() const noexcept { return m_KeyCode; }
	};

public:
	FxKeyboard()							 = default;
	FxKeyboard(const FxKeyboard&)			 = delete;
	FxKeyboard& operator=(const FxKeyboard&) = delete;

	//~ Key Events
	bool	IsKeyPressed(unsigned char keyCode) const noexcept;
	FxEvent ReadKey() noexcept;
	bool	IsKeyEmpty() const noexcept;
	void	FlushKey() noexcept;

	//~ Char Event
	char ReadChar()		noexcept;
	bool IsCharEmpty()  const noexcept;
	void FlushChar()	noexcept;

	void Flush()		noexcept;

	//~ Repeat Control
	void EnableAutoRepeat() noexcept;
	void DisableAutoRepear() noexcept;
	bool IsAutoRepeatEnabled() const noexcept;

private:
	//~ Managed by FxWindows
	void OnKeyPressed(unsigned char keycode)  noexcept;
	void OnKeyReleased(unsigned char keycode) noexcept;
	void OnChar(char character) noexcept;
	void ClearState() noexcept;

	template<typename T>
	static void TrimBuffer(std::queue<T>& buffer) noexcept;

private:
	static constexpr uint32_t m_uiKeys		{ 256u };
	static constexpr uint32_t m_uiBufferSize{ 16u };
	bool m_bRepeatEnabled{ false };

	std::bitset<m_uiKeys>	m_Keystates;
	std::queue<FxEvent>		m_KeyEventBuffer;
	std::queue<char>		m_CharBuffer;
};

template<typename T>
inline void FxKeyboard::TrimBuffer(std::queue<T>& buffer) noexcept
{
	while (buffer.size() >= m_uiBufferSize)
	{
		buffer.pop();
	}
}
