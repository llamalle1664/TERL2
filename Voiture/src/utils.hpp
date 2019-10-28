#ifndef DEF_UTILS_HPP
#define DEF_UTILS_HPP

constexpr float agent_radius = 25.f;  // agent radius, in pixels
constexpr float agent_speed = 1000.f;  // agent speed, in pixel/s
constexpr float agent_turn_speed = 500.f; // agent turn speed, in degree/s
constexpr int   agent_point_count = 32; // number of points for the agent circle, relevant only for renderer
constexpr int   agent_sensor_count = 5;
constexpr float agent_sensor_range = 3*agent_radius; // sensor range, in pixel

constexpr float world_step = 1.f/60.f;    // world time step in seconds
constexpr int   world_worker_count = 16;   // number of threads used to step the world
constexpr float world_worker_sim_max_time = 60.f; // max amount of simulation time, in seconds
constexpr float world_worker_timeout = 10.f; // timeout for the worker threads, maximum time to reach next checkpoint, in seconds

constexpr int autorefresh_seconds = 10; // last generation autorefresh interval

//NeuralNetwork
constexpr float NNetwork_randomFactor = 1.f; // how far from 0 can the values for a new neural network be
constexpr float NNetwork_randomFactorMut = 0.05f; //mutation strenght
  //Used to mix genes from 2 NeuralNetwork
constexpr int NNetwork_minMixNeuron = 5; //The minimum amount of neuron genes that can be copied
constexpr int NNetwork_maxMixNeuron = 20; //The minimum amount of neuron genes that can be copied
constexpr int NNetwork_minMixWeight = 5; //The minimum amount of weight genes that can be copied
constexpr int NNetwork_maxMixWeight = 50; //The minimum amount of weight genes that can be copied

constexpr int NNetwork_minMut = 30; //The minimum amount of unaltered genes between mutations
constexpr int NNetwork_maxMut = 300; //The maximum amount of unaltered genes between mutations

//Simulator
  //Generation
constexpr int sim_genElders = 20;
constexpr int sim_genChilds = 74;
constexpr int sim_genRandom = 1;
constexpr int sim_genTotal = sim_genElders + sim_genChilds + sim_genRandom;

#endif
