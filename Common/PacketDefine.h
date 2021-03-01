#ifndef PACKET_DEFINE_H
#define PACKET_DEFINE_H
#pragma pack(1)
struct CUSTOM_PACKET_HEAD
{
	uint8 headcode;
	uint16 size;
	uint16 server;

	void set(uint8 headcode, uint16 size)
	{
		this->headcode = headcode;
		this->size = size;
		this->server = -1;
	}

	uint16 get_size() const { return this->size; }
	uint16 GetServerGroup() const { return this->server / 20; }
};

struct CUSTOM_PACKET_PLAYER
{
	void Reset()
	{
		this->entry = -1;
		this->ms_time = 0;
		this->account_id = 0;
		this->guid = 0;
		memset(this->name, 0, MAX_CHARACTER_LENGTH + 1);
	}

	uint16 entry;
	uint32 ms_time;
	uint32 account_id;
	uint32 guid;
	char name[MAX_CHARACTER_LENGTH + 1];
};
#pragma pack()
struct PBMSG_HEAD
{
public:
	void set(uint8 head, uint8 size)
	{
		this->c = 0xC1;
		this->size = size;
		this->headcode = head;
	};

	void setE(uint8 head, uint8 size)
	{
		this->c = 0xC3;
		this->size = size;
		this->headcode = head;
	};

	uint8 get_size() const { return this->size; }

	uint8 c;
	uint8 size;
	uint8 headcode;
};

struct PWMSG_HEAD
{
public:
	void set(uint8 head, uint16 size)
	{
		this->c = 0xC2;
		this->sizeH = SET_NUMBERH(size);
		this->sizeL = SET_NUMBERL(size);
		this->headcode = head;
	};

	void setE(uint8 head, uint16 size)
	{
		this->c = 0xC4;
		this->sizeH = SET_NUMBERH(size);
		this->sizeL = SET_NUMBERL(size);
		this->headcode = head;
	};

	void set_size(uint16 size)
	{
		this->sizeH = SET_NUMBERH(size);
		this->sizeL = SET_NUMBERL(size);
	}

	uint16 get_size() { return MAKE_NUMBERW(this->sizeH,this->sizeL); }

	uint8 c;
	uint8 sizeH;
	uint8 sizeL;
	uint8 headcode;
};

struct PBMSG_HEAD2
{
public:

	void set(uint8 head, uint8 sub, uint8 size)
	{
		this->c = 0xC1;
		this->size = size;
		this->headcode = head;
		this->subcode = sub;
	};

	void setE(uint8 head, uint8 sub, uint8 size)
	{
		this->c = 0xC3;
		this->size = size;
		this->headcode = head;
		this->subcode = sub;
	};

	uint8 get_size() const { return this->size; }

	uint8 c;
	uint8 size;
	uint8 headcode;
	uint8 subcode;
};

struct PWMSG_HEAD2
{
public:
	void set(uint8 head, uint8 sub, uint16 size)
	{
		this->c = 0xC2;
		this->sizeH = SET_NUMBERH(size);
		this->sizeL = SET_NUMBERL(size);
		this->headcode = head;
		this->subcode = sub;
	}

	void setE(uint8 head, uint8 sub, uint16 size)
	{
		this->c = 0xC4;
		this->sizeH = SET_NUMBERH(size);
		this->sizeL = SET_NUMBERL(size);
		this->headcode = head;
		this->subcode = sub;
	}

	void set_size(uint16 size)
	{
		this->sizeH = SET_NUMBERH(size);
		this->sizeL = SET_NUMBERL(size);
	}

	uint16 get_size() const { return MAKE_NUMBERW(this->sizeH,this->sizeL); }

	uint8 c;
	uint8 sizeH;
	uint8 sizeL;
	uint8 headcode;
	uint8 subcode;
};

#pragma pack(1)
struct C1_HEADER
{
public:

	void Set(uint8 head, uint8 size)
	{
		this->pct_id = 0xC1;
		this->pct_size = size;
		this->pct_headcode = head;
	};

	uint8 GetSize() const { return this->pct_size; }

	uint8 pct_id;
	uint8 pct_size;
	uint8 pct_headcode;
};

struct C1_HEADER_SUB : C1_HEADER
{
public:

	void Set(uint8 head, uint8 sub, uint8 size)
	{
		this->C1_HEADER::Set(head, size);
		this->pct_subcode = sub;
	};

	uint8 pct_subcode;
};

struct C2_HEADER
{
public:

	void Set(uint8 head, uint16 size)
	{
		this->pct_id = 0xC2;
		this->SetSize(size);
		this->pct_headcode = head;
	};

	void SetSize(uint16 size)
	{
		this->pct_size[0] = SET_NUMBERH(size);
		this->pct_size[1] = SET_NUMBERL(size);
	}

	uint16 GetSize() const { return MAKE_NUMBERW(this->pct_size[0], this->pct_size[1]); }

	uint8 pct_id;
	uint8 pct_size[2];
	uint8 pct_headcode;
};

struct C2_HEADER_SUB : C2_HEADER
{
public:

	void Set(uint8 head, uint8 sub, uint16 size)
	{
		this->C2_HEADER::Set(head, size);
		this->pct_subcode = sub;
	};

	uint8 pct_subcode;
};
#pragma pack()

#endif