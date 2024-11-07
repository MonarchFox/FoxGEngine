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
			INVALID
		};

	private:
		EventType mEventType;
		bool m_bLeftPressed;
		bool m_bRightPressed;
		bool m_bWheelPressed;
		int m_PositionX, m_PositionY;

	public:
		FxEvent() : mEventType(EventType::INVALID), m_bLeftPressed(false), m_bWheelPressed(false),
					m_bRightPressed(false), m_PositionX(0), m_PositionY(0)
		{}

		FxEvent(EventType eventType, bool leftPressed, bool rightPressed, bool wheelPressed, int x, int y)
			: mEventType(eventType), m_bLeftPressed(leftPressed), m_bWheelPressed(wheelPressed),
			  m_bRightPressed(rightPressed), m_PositionX(x), m_PositionY(y)
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

	bool IsLeftPressed() const noexcept;
	bool IsRightPressed() const noexcept;
	bool IsMousePressed() const noexcept;

	FxMouse::FxEvent Read() noexcept;

	void Flush() noexcept;
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

	void TrimBuffer() noexcept;

private:
	std::queue<FxEvent> m_qBuffer;
	static constexpr unsigned int m_BufferSize{ 16u };
	int		m_iPositionX{ 0 };
	int		m_iPositionY{ 0 };
	bool	m_bLeftPressed { false };
	bool	m_bRightPressed{ false };
	bool	m_bMousePressed{ false };
};
