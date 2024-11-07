#include "../Public/FxMouse.h"
#include "../Public/FxWinCore.h"

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

bool FxMouse::IsMouseInside() const noexcept
{
	return m_bMouseInside;
}

FxMouse::FxEvent FxMouse::Read() noexcept
{
	if (!IsEmpty())
	{
		FxMouse::FxEvent e = m_qBuffer.front();
		m_qBuffer.pop();
		return e;
	}
	return FxEvent(FxEvent::EventType::INVALID, *this);
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
	m_qBuffer.push(FxEvent(FxEvent::EventType::MOVE, *this));
	TrimBuffer();
}

void FxMouse::OnLeftMousePressed(int x, int y) noexcept
{
	m_bLeftPressed = true;
	m_qBuffer.push(FxEvent(FxEvent::EventType::L_PRESS, *this));
	TrimBuffer();
}

void FxMouse::OnLeftMouseRelased(int x, int y) noexcept
{
	m_bLeftPressed = false;
	m_qBuffer.push(FxEvent(FxEvent::EventType::L_RELEASE, *this));
	TrimBuffer();
}

void FxMouse::OnRightMousePressed(int x, int y) noexcept
{
	m_bRightPressed = true;
	m_qBuffer.push(FxEvent(FxEvent::EventType::R_PRESS, *this));
	TrimBuffer();
}

void FxMouse::OnRightMouseRelased(int x, int y) noexcept
{
	m_bRightPressed = true;
	m_qBuffer.push(FxEvent(FxEvent::EventType::R_RELEASE, *this));
}

void FxMouse::OnWheelMouseUp(int x, int y) noexcept
{
	m_qBuffer.push(FxEvent(FxEvent::EventType::WHEEL_UP, *this));
	TrimBuffer();
}

void FxMouse::OnWheelMouseDown(int x, int y) noexcept
{
	m_qBuffer.push(FxEvent(FxEvent::EventType::WHEEL_DOWN, *this));
	TrimBuffer();
}

void FxMouse::OnWheelMousePressed(int x, int y) noexcept
{
	m_bMousePressed = true;
	m_qBuffer.push(FxEvent(FxEvent::EventType::WHEEL_PRESS, *this));
	TrimBuffer();
}

void FxMouse::OnWheelMouseReleased(int x, int y) noexcept
{
	m_bMousePressed = false;
	m_qBuffer.push(FxEvent(FxEvent::EventType::WHEEL_RELEASE, *this));
	TrimBuffer();
}

void FxMouse::OnMouseLeave() noexcept
{
	m_bMouseInside = false;
	m_qBuffer.push(FxEvent(FxEvent::EventType::LEAVE, *this));
}

void FxMouse::OnMouseEnter() noexcept
{
	m_bMouseInside = true;
	m_qBuffer.push(FxEvent(FxEvent::EventType::ENTER, *this));
}

void FxMouse::OnWheelDelta(int x, int y, int delta) noexcept
{
	m_iDelta += delta;

	while (m_iDelta >= WHEEL_DELTA)
	{
		m_iDelta -= WHEEL_DELTA;
		OnWheelMouseUp(x, y);
	}

	while (m_iDelta <= WHEEL_DELTA)
	{
		m_iDelta += WHEEL_DELTA;
		OnWheelMouseDown(x, y);
	}
}

void FxMouse::TrimBuffer() noexcept
{
	while (m_qBuffer.size() >= m_BufferSize)
	{
		m_qBuffer.pop();
	}
}
