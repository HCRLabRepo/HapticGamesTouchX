#pragma once

class SensorData {
	public:
		SensorData();
		void getData();
		virtual ~SensorData() {};

	public:
		float conductance;

};
