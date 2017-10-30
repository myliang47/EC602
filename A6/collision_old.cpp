//Copyright 2017 Sihan Wang shwang95@bu.edu
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <math.h>
#include <tuple>

using namespace std;

class Ball {
// Class ball for store balls' information
public:

	string id;
	long double px, py, vx, vy;

	void set(string s, long double xp, long double yp, long double xv, long double yv) {
		id = s;
		px = xp;
		py = yp;
		vx = xv;
		vy = yv;
	}

	void positionAtTime(long double time) {
		px += time * vx;
		py += time * vy;
	}

	void printBall() {
		cout.precision(8);
		cout << id << " ";
		cout << px << " ";
		cout << py << " ";
		cout << vx << " ";
		cout << vy << endl;
	}

	void updateV(long double xv, long double yv) {
		vx = xv;
		vy = yv;
	}

	bool operator == (const Ball& rhs) const {
		return id == rhs.id && px == rhs.px && py == rhs.py && vx == rhs.vx && vy == rhs.vy;
	}

};

class Collisions {
// Class collisions for store collisions' information
public:

	vector<Ball> balls;
	long double time;

	void set(Ball a, Ball b, long double t) {
		balls.push_back(a);
		balls.push_back(b);
		time = t;
	}

	Ball getA() {
		return balls[0];
	}

	Ball getB() {
		return balls[1];
	}

};

long double collisionTimeDetect(Ball ball_1, Ball ball_2) {
	// Predict the collision time of two balls
	vector<long double> collisionTime;
	long double dx = ball_2.px - ball_1.px;
	long double dy = ball_2.py - ball_1.py;
	long double dvx = ball_2.vx - ball_1.vx;
	long double dvy = ball_2.vy - ball_1.vy;
	long double a = pow(dvx, 2) + pow(dvy, 2);
	long double b = 2 * (dvx * dx + dvy * dy);
	long double c = pow(dx, 2) + pow(dy, 2) - 100;
	long double d = pow(b, 2) - 4 * a * c;

	if(d >= 0 && a != 0) {
		if((- b + sqrt(d)) / (2 * a) > 0)
			collisionTime.push_back((- b + sqrt(d)) / (2 * a));

		if((- b - sqrt(d)) / (2 * a) > 0)
			collisionTime.push_back((- b - sqrt(d)) / (2 * a));
	}

	if(collisionTime.size() > 0) {
		auto minTime = *min_element(collisionTime.begin(), collisionTime.end());
		return minTime;
	} else
		return -1;
}

vector<Collisions> collisionDetect(vector<Ball> balls) {
	// Detect all potential collisions of all balls in pair
	vector<Collisions> collisionTimes;

	for(int i = 0; i < balls.size(); i++) {
		for(int j = i + 1; j < balls.size(); j++) {
			if(i != j && collisionTimeDetect(balls[i], balls[j]) != -1) {
				Collisions c;
				c.set(balls[i], balls[j], collisionTimeDetect(balls[i], balls[j]));
				collisionTimes.push_back(c);
			}
		}
	}

	return collisionTimes;
}

tuple<long double, long double, long double, long double> collision(Ball ball_1, Ball ball_2) {
	// Handle velocity changes when collision
	long double x1 = ball_1.px;
	long double y1 = ball_1.py;
	long double vx1 = ball_1.vx;
	long double vy1 = ball_1.vy;
	long double x2 = ball_2.px;
	long double y2 = ball_2.py;
	long double vx2 = ball_2.vx;
	long double vy2 = ball_2.vy;
	long double dot1 = ((vx1 - vx2) * (x1 - x2) + (vy1 - vy2) * (y1 - y2)) / (pow((x1 - x2), 2) + pow((y1 - y2), 2));
	long double dot2 = ((vx2 - vx1) * (x2 - x1) + (vy2 - vy1) * (y2 - y1)) / (pow((x2 - x1), 2) + pow((y2 - y1), 2));
	return make_tuple(vx1 - dot1 * (x1 - x2), vy1 - dot1 * (y1 - y2), vx2 - dot2 * (x2 - x1), vy2 - dot2 * (y2 - y1));
}

bool isNum(string s) {
	istringstream iss(s);
	long double dummy;
	iss >> noskipws >> dummy;
	return iss && iss.eof();
}

int main(int argc, char **argv) {
	vector<int> times;

	if(argc == 1)
		exit(2);

	for(int i = 1; i < argc; i++) {
		if(!isNum(argv[i]))
			exit(2);

		if(stoi(argv[i]) >= 0)
			times.push_back(stoi(argv[i]));
	}

	if(times.size() == 0)
		exit(2);

	sort(times.begin(), times.end());
	string line;
	vector<vector<string>> prop;

	int flag = 0;
	int rc = 0;

	while(!cin.eof()) {
		getline(cin, line);

		if(line != "") {
			istringstream iss(line);
			vector<string> propline{istream_iterator<string>(iss), istream_iterator<string>()};
			prop.push_back(propline);
		}
		else {
			if(flag == 0)
				flag = 1;
			else
				rc = 1;
		}
	}

	vector<Ball> balls;

	for(int i = 0; i < prop.size(); i++) {
		if(prop[i].size() != 5)
			exit(1);

		for(int j = 1; j < prop[i].size(); j++) {
			if(!isNum(prop[i][j]))
				exit(1);
		}

		Ball temp;
		temp.set(prop[i][0], stod(prop[i][1]), stod(prop[i][2]), stod(prop[i][3]), stod(prop[i][4]));
		balls.push_back(temp);
	}

	long double time = 0;  // Time period starting zero point of the loop

	for(int j = 0; j < times.size(); j++) {
		// Start loop
		cout << times[j] << endl;  // Display time

		vector<Collisions> collisionTimes;

		if(collisionDetect(balls).size() != 0) {
			// If have potential collisions
			collisionTimes = collisionDetect(balls);
			sort(collisionTimes.begin(), collisionTimes.end(), [](const Collisions & lhs, const Collisions & rhs) {
				return lhs.time < rhs.time;
			});

			for(int k = 0; k < collisionTimes.size(); k ++) {
				if(collisionTimes[k].time <= times[j] - time) {
					// If predicted collision time occurs inside the time period
					int a, b;
					for(int i = 0; i < balls.size(); i++) {
						// Update velocity
						if(balls[i] == collisionTimes[k].getA())
							a = i;
						else if(balls[i] == collisionTimes[k].getB())
							b = i;
					}

					for(int i = 0; i < balls.size(); i++)
						balls[i].positionAtTime(collisionTimes[k].time);  // Collision position

					tuple<long double, long double, long double, long double> temp = collision(collisionTimes[k].getA(), collisionTimes[k].getB());  // Do collide

					balls[a].updateV(get<0>(temp), get<1>(temp));
					balls[b].updateV(get<2>(temp), get<3>(temp));

					time += collisionTimes[k].time;  // Add time period of collision

					if(collisionDetect(balls).size() != 0) {
						collisionTimes = collisionDetect(balls);
						sort(collisionTimes.begin(), collisionTimes.end(), [](const Collisions & lhs, const Collisions & rhs) {
							return lhs.time < rhs.time;
						});
						k = -1;
					}
				}
			}

			for(int i = 0; i < balls.size(); i++) {
				//Calculate position of the end of the time period
				balls[i].positionAtTime(times[j] - time);
				balls[i].printBall();
			}
		} else {
			// If no potential collisions
			for(int i = 0; i < balls.size(); i++) {
				balls[i].positionAtTime(times[j] - time);
				balls[i].printBall();
			}
		}

		time = times[j]; // Update time period zero point
	}

	exit(rc);
}