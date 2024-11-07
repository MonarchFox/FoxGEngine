#include "../Public/FxMouse.h"

std::pair<int, int> FxMouse::GetPosition() const noexcept
{
	return {m_iPositionX, m_iPositionY};
}

int FxMouse::GetPositionX() const noexcept
{
	return m_iPositionX;
}

int FxMouse::GetPositionY() const noexcept
{
	return m_iPositionY;
}

bool FxMouse::IsLeftPressed() const noexcept
{
	return m_bLeftPressed;
}

bool FxMouse::IsRightPressed() const noexcept
{
	return m_bRightPressed;
}

bool FxMouse::IsMousePressed() const noexcept
{
	return m_bMousePressed;
}

FxMouse::FxEvent FxMouse::Read() noexcept
{
	if (!IsEmpty())
	{
		FxMouse::FxEvent e = m_qBuffer.front();
		m_qBuffer.pop();
		return e;
	}
	return FxMouse::FxEvent();
}

void FxMouse::Flush() noexcept
{
	m_qBuffer = std::queue<FxEvent>();
}

bool FxMouse::IsEmpty() const noexcept
{
	return m_qBuffer.empty();
}

void FxMouse::OnMouseMove(int x, int y) noexcept
{
	m_qBuffer.push(FxEvent(FxEvent::EventType::MOVE, false, false, false, x, y));
	TrimBuffer();
}

void FxMouse::OnLeftMousePressed(int x, int y) noexcept
{
	m_qBuffer.push(FxEvent(FxEvent::EventType::L_PRESS, true, false, false, x, y));
	m_bLeftPressed = true;
	TrimBuffer();
}

void FxMouse::OnLeftMouseRelased(int x, int y) noexcept
{
	m_qBuffer.push(FxEvent(FxEvent::EventType::L_RELEASE, false, false, false, x, y));
	m_bLeftPressed = false;
	TrimBuffer();
}

void FxMouse::OnRightMousePressed(int x, int y) noexcept
{
	m_qBuffer.push(FxEvent(FxEvent::EventType::R_PRESS, false, true, false, x, y));
	m_bRightPressed = true;
	TrimBuffer();
}

void FxMouse::OnRightMouseRelased(int x, int y) noexcept
{
	m_qBuffer.push(FxEvent(FxEvent::EventType::R_RELEASE, false, false, false, x, y));
	m_bRightPressed = true;
}

void FxMouse::OnWheelMouseUp(int x, int y) noexcept
{
	m_qBuffer.push(FxEvent(FxEvent::EventType::WHEEL_UP, false, false, false, x, y));
	TrimBuffer();
}

void FxMouse::OnWheelMouseDown(int x, int y) noexcept
{
	m_qBuffer.push(FxEvent(FxEvent::EventType::WHEEL_DOWN, false, false, false, x, y));
	TrimBuffer();
}

void FxMouse::OnWheelMousePressed(int x, int y) noexcept
{
	m_qBuffer.push(FxEvent(FxEvent::EventType::WHEEL_PRESS, false, false, true, x, y));
	m_bMousePressed = true;
	TrimBuffer();
}

void FxMouse::OnWheelMouseReleased(int x, int y) noexcept
{
	m_qBuffer.push(FxEvent(FxEvent::EventType::WHEEL_RELEASE, false, false, false, x, y));
	m_bMousePressed = false;
	TrimBuffer();
}

void FxMouse::TrimBuffer() noexcept
{
	while (m_qBuffer.size() >= m_BufferSize)
	{
		m_qBuffer.pop();
	}
}
