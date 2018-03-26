#pragma once

namespace FlappyBird
{
	class AnimTimer
	{
		typedef /*std::chrono::milliseconds*/int64 InternalDuration;
	public:
		AnimTimer()
		{
			length = 0.f;
			SetTime( 0.f );
		}

		void SetLength( float _length )
		{
			if ( _length > 0 )
				length = _length;
			else
				length = 0.f;
		}

		float GetLength() const
		{ 
			return length;
		}

		void SetTime( float time )
		{
			referenceTime = InternalDuration(getTimeInMilliseconds() - (int64)(time * 1000));
		}

		float GetTime() const
		{
			return (getTimeInMilliseconds() - (int64)(referenceTime)) * 0.001f;
		}

		bool IsOvertimed() const
		{
			return GetTime() > GetLength();
		}

		float GetOvertime() const
		{
			float overtime = GetTime() - length;
			return (overtime > 0.f) ? overtime : 0.f;
		}

		float GetProgress() const
		{
			if ( length > 0.f )
				return GetTime() / length;
			return 0.f;
		}

		float GetPingPongAnim()
		{
			uint n = (int64)(GetTime() * 1000);
			uint t = (int64)(length * 1000);
			int cycle = n / t;
			n = n % t;
			if ((cycle % 2) == 1)
				n = t - n - 1;
			return (n * 0.001f - length) / length;
		}
	
	private:
		InternalDuration	referenceTime;
		float				length;
	};
}

