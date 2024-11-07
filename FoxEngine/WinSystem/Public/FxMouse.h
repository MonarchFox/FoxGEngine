#pragma once

#include <queue>


class FxMouse
{
	friend class FxWindow;

public:
	//~ Mouse Events 
	class FxEvent
	{
	public:
		enum class EventType: unsigned int
		{
			L_PRESS,
			L_RELEASE,
			R_PRESS,
			R_RELEASE,
			WHEEL_UP,
			WHEEL_DOWN,
			WHEEL_PRESS,
			WHEEL_RELEASE,
			MOVE,
			INVALID,
			ENTER,
			LEAVE,
		};

	private:
		EventType mEventType;
		bool m_bLeftPressed;
		bool m_bRightPressed;
		bool m_bWheelPressed;
		int m_PositionX, m_PositionY;

	public:
		FxEvent(EventType eventType, const FxMouse& parent)
			: mEventType(eventType), m_bLeftPressed(parent.IsLeftPressed()), m_bWheelPressed(parent.IsMousePressed()),
			  m_bRightPressed(parent.IsRightPressed()), m_PositionX(GetPositionX()), m_PositionY(GetPositionY())
		{}

		std::pair<int, int> GetPosition() const noexcept { return { m_PositionX, m_PositionY }; }
		
		bool IsValid()	const noexcept { return mEventType != EventType::INVALID; }

		EventType	GetType()		  const noexcept { return mEventType;	   }
		int			GetPositionY()	  const noexcept { return m_PositionY;	   }
		int			GetPositionX()	  const noexcept { return m_PositionX;	   }
		bool		IsLeftPressed()	  const noexcept { return m_bLeftPressed;  }
		bool		IsRightPressed()  const noexcept { return m_bRightPressed; }
		bool		IsWheelPressed()  const noexcept { return m_bWheelPressed; }
	};

public:
	//~ Mouse Class
public:
	FxMouse() {}
	FxMouse(const FxMouse&) = delete;
	FxMouse& operator=(const FxMouse&) = delete;
	
	std::pair<int, int> GetPosition() const noexcept;
	int GetPositionX() const noexcept;
	int GetPositionY() const noexcept;

	bool IsLeftPressed()  const noexcept;
	bool IsRightPressed() const noexcept;
	bool IsMousePressed() const noexcept;
	bool IsMouseInside()  const noexcept;

	FxMouse::FxEvent Read() noexcept;

	void Flush()   noexcept;
	bool IsEmpty() const noexcept;

private:
	void OnMouseMove(int x, int y) noexcept;

	void OnLeftMousePressed(int x, int y) noexcept;
	void OnLeftMouseRelased(int x, int y) noexcept;

	void OnRightMousePressed(int x, int y) noexcept;
	void OnRightMouseRelased(int x, int y) noexcept;

	void OnWheelMouseUp(int x, int y) noexcept;
	void OnWheelMouseDown(int x, int y) noexcept;

	void OnWheelMousePressed(int x, int y) noexcept;
	void OnWheelMouseReleased(int x, int y) noexcept;

	void OnMouseLeave() noexcept;
	void OnMouseEnter() noexcept;

	void OnWheelDelta(int x, int y, int delta) noexcept;

	void TrimBuffer() noexcept;

private:
	std::queue<FxEvent> m_qBuffer;
	static constexpr unsigned int m_BufferSize{ 16u };
	int		m_iPositionX{ 0 };
	int		m_iPositionY{ 0 };
	int		m_iDelta	{ 0 };
	bool	m_bLeftPressed { false };
	bool	m_bRightPressed{ false };
	bool	m_bMousePressed{ false };
	bool	m_bMouseInside { false };
};
