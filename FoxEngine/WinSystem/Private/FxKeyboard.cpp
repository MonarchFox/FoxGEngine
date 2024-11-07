#include "../Public/FxKeyboard.h"


bool FxKeyboard::IsKeyPressed(unsigned char keyCode) const noexcept
{
	return m_Keystates[keyCode];
}

FxKeyboard::FxEvent FxKeyboard::ReadKey() noexcept
{
	if (!IsKeyEmpty())
	{
		FxKeyboard::FxEvent e = m_KeyEventBuffer.front();
		m_KeyEventBuffer.pop();
		return e;
	}
	return FxKeyboard::FxEvent();
}

bool FxKeyboard::IsKeyEmpty() const noexcept
{
	return m_KeyEventBuffer.empty();
}

void FxKeyboard::FlushKey() noexcept
{
	m_KeyEventBuffer = std::queue<FxKeyboard::FxEvent>();
}

char FxKeyboard::ReadChar() noexcept
{
	if (!IsCharEmpty())
	{
		unsigned char charCode = m_CharBuffer.front();
		m_CharBuffer.pop();
		return charCode;
	}
	return 0;
}

bool FxKeyboard::IsCharEmpty() const noexcept
{
	return m_CharBuffer.empty();
}

void FxKeyboard::FlushChar() noexcept
{
	m_CharBuffer = std::queue<char>();
}

void FxKeyboard::Flush() noexcept
{
	FlushKey();
	FlushChar();
}

void FxKeyboard::EnableAutoRepeat() noexcept
{
	m_bRepeatEnabled = true;
}

void FxKeyboard::DisableAutoRepear() noexcept
{
	m_bRepeatEnabled = false;
}

bool FxKeyboard::IsAutoRepeatEnabled() const noexcept
{
	return m_bRepeatEnabled;
}

void FxKeyboard::OnKeyPressed(unsigned char keycode) noexcept
{
	m_Keystates[keycode] = true;
	m_KeyEventBuffer.push(FxKeyboard::FxEvent(FxKeyboard::FxEvent::KeyType::PRESS, keycode));
	TrimBuffer(m_KeyEventBuffer);
}

void FxKeyboard::OnKeyReleased(unsigned char keycode) noexcept
{
	m_Keystates[keycode] = false;
	m_KeyEventBuffer.push(FxKeyboard::FxEvent(FxKeyboard::FxEvent::KeyType::RELEASE, keycode));
	TrimBuffer(m_KeyEventBuffer);
}

void FxKeyboard::OnChar(char character) noexcept
{
	m_CharBuffer.push(character);
	TrimBuffer(m_KeyEventBuffer);
}

void FxKeyboard::ClearState() noexcept
{
	m_Keystates = std::bitset<m_uiKeys>();
}
