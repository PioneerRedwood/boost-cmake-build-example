/**
 * @file message.hpp
 * @version 0.1
 * @date 2022-01-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once

#include <vector>
#include <ostream>

namespace ban {

struct vector2
{
	float x;
	float y;
public:
	std::string to_string() { return "x:" + std::to_string(x) + " y:" + std::to_string(y); }
	static size_t size() { return sizeof(float) * 2; }
};

struct vector3
{
	float x;
	float y;
	float z;
public:
	std::string to_string() { return "x:" + std::to_string(x) + " y:" + std::to_string(y) + " z:" + std::to_string(z); }
	static size_t size() { return sizeof(float) * 3; }
};

template<typename T>
struct MessageHeader {
  uint32_t size_ = 0;
  T id_;
};

template<typename T>
struct Message
{
	MessageHeader<T> header_{};
	std::vector<uint8_t> body_{};

	std::size_t size() const { return body_.size(); }

	friend std::ostream& operator << (std::ostream& os, const Message<T>& msg) {
		os << "ID:" << uint32_t(msg.header_.id_) << " Size:" << msg.header_.size_;
		return os;
	}

	template<typename DataType>
	friend Message<T>& operator << (Message<T>& msg, const DataType& data) {
		static_assert(std::is_standard_layout_v<DataType>, "Data is too complex to be pushed into vector");

		size_t i = msg.body_.size();

		msg.body_.resize(msg.body_.size() + sizeof(DataType));

		std::memcpy(msg.body_.data() + i, &data, sizeof(DataType));

		msg.header_.size_ = msg.size();

		return msg;
	}

	template<typename DataType>
	friend Message<T>& operator >> (Message<T>& msg, DataType& data) {
		static_assert(std::is_standard_layout_v<DataType>, "Data is too complex to be pulled from vector");

		size_t i = msg.body_.size() - sizeof(DataType);

		std::memcpy(&data, msg.body_.data() + i, sizeof(DataType));

		msg.body_.resize(i);

		msg.header_.size_ = msg.size();

		return msg;
	}

	// Write string data
	void Write(const std::string& data) {
		std::vector<uint8_t> temp(data.begin(), data.end());

		body_.insert(body_.end(), temp.begin(), temp.end());
		header_.size_ = body_.size();
	}

	// Write vector2
	void Write(const vector2& data) {
		*this << data.x << data.y;
	}

	// Write vector3
	void Write(const vector3& data) {
		*this << data.x << data.y << data.z;
	}

	// Read data from `start_index` about `count`
	void Read(std::string& data, int start_index, int count) {
		if ((int)body_.size() < (count - start_index)) {
			return;
		}

		try {
			data = std::string((char*)body_.data() + start_index, count);
		}
		catch (const std::exception& e) {
			std::cerr << e.what() << "\n";
			return;
		}
	}

	// Read vector2
	void Read(vector2& data, int start_index) {
		Read(data.x, start_index);
		Read(data.y, start_index + sizeof(float));
	}

	// Read vector3
	void Read(vector3& data, int start_index) {
		Read(data.x, start_index);
		Read(data.y, start_index + sizeof(float));
		Read(data.z, start_index + sizeof(float) + sizeof(float));
	}

	// Read data, with starting given `start_index`
	template<typename DataType>
	void Read(DataType& data, int start_index) {
		static_assert(std::is_standard_layout_v<DataType>, "Data is too complex to be pulled from vector");
		std::memcpy(&data, body_.data() + start_index, sizeof(DataType));
	}
};

enum class LobbyMsg : uint32_t {
  HEARTBEAT = 0,

};

}