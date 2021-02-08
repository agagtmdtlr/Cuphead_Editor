#pragma once

enum class PlayMode
{
	End = 0, Loop, Reverse
};

struct Frame
{
	float Time;
	Sprite* Image;

	Frame(Sprite* sprite, float time);
	~Frame();
};

class Clip
{
public:
	Clip(PlayMode mode = PlayMode::End, float speed = 1.0f);
	~Clip();

	void Position(float x, float y);
	void Position(D3DXVECTOR2& vec);
	D3DXVECTOR2 Position();

	void Scale(float x, float y);
	void Scale(D3DXVECTOR2& vec);
	D3DXVECTOR2 Scale();


	void Rotation(float x, float y, float z);
	void Rotation(D3DXVECTOR3& vec);
	D3DXVECTOR3 Rotation();


	void RotationDegree(float x, float y, float z);
	void RotationDegree(D3DXVECTOR3& vec);
	D3DXVECTOR3 RotationDegree();


	D3DXVECTOR2 TextureSize();

	void AddFrame(Sprite* sprite, float time);

	void Play();
	void Play(UINT startFrame);
	void Stop();

	void Update(D3DXMATRIX& V, D3DXMATRIX& P);
	void Render();

	void DrawBound(bool val);

	Sprite* GetSprite();

	UINT CurrentFrame() { return currentFrame; }

private:
	D3DXVECTOR2 position;
	D3DXVECTOR2 scale;
	D3DXVECTOR3 rotation;

	float length;
	float speed;

	bool bPlay;
	UINT currentFrame;
	float playTime;

	PlayMode mode;

	vector<Frame *> frames;
};