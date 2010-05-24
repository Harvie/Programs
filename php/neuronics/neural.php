#!/usr/bin/php
<?php
//NMNN - Nervous Man's Neural Network
//Harvie 2oo7/8
/*
 * My first neural network
 */

///SETTINGS/INIT
$nmnn_version = '0.1-DEV';


///FUNCTIONS///////////////////////////////////////////////////////////////////////////////////////

function nervous_banner($text) { //Print banner with custom text
	echo("\n[*]-== Nervous Man's Neural Network ==-[*]\n");	
	$text = explode("\n", $text);
	foreach($text as $line) {
		$line = trim($line);
		echo("                                     =-[+]\r");
		echo("[+]-= $line\n");
	}
	echo("[*]-==================================-[*]\n\n");
}

function neural_network_create($net) { //Create new network in $GLOBALS['neural_networks'][$net]
	if(isset($GLOBALS['neural_networks'][$net])) {
		echo("Net already exists\n");
		return(0);
	}
	$GLOBALS['neural_networks'][$net] = new neural_network($net);
	return(1);
}

function neural_network($net) {
	return($GLOBALS['neural_networks'][$net]);
}

function global_neuron($net, $id) {
	return neural_network($net)->neurons[$id];
}

///CLASSES/////////////////////////////////////////////////////////////////////////////////////////

class neuron { //Single Neurone class

	var $net = 'N/A'; //ID of network containing this neuron
	var $id = 'N/A'; //ID of this neuron
	var $treshold = 0; //Treshold is sensitivity of this neuron
	var $synapses = array(); //List of neurons connected to dendrites (inputs)
	var $decisions = array(); //List of values received from dendrites
	var $axon = 'N/A'; //Final decision of this neuron
	var $sensor = false; //Is this a sensor?

	function neuron($net, $id, $treshold=0) { //Constructor - Set neuron identification
		$this->net = $net;
		$this->id = $id;
		$this->treshold = $treshold;
	}
	
	function synapse_add($id, $strength=0) { //Connect neuron to dendrite
		$this->synapses[$id] = $strength;
	}
	
	function synapse_request($id) { //Request synapse add by another neuron
		if(!is_array($id)) {
			global_neuron($this->net, $id)->synapse_add($this->id);
		} else foreach($id as $id_single) {
			global_neuron($this->net, $id_single)->synapse_add($this->id);
		}
	}

	function synapse_break($id) { //Disconnect neuron from dendrite
		unset($this->synapses[$id]);
	}

	function debug() { //Print info about this neuron
		echo("[i] Info about neuron no.$this->id in $this->net network:\n");
		if($this->sensor) { //if this is sensor print different stuff
			echo("\tThis cell is a SENSOR!!!\n\tSensor axon value:$this->axon\n\n");
			return('s');
		}
		echo("\tTreshold: $this->treshold\n\tAxon: $this->axon\n");
		echo("\tSynapses:\n");
		print_r($this->synapses);
		echo("\tDecisions:\n");
		print_r($this->decisions);
		echo("\n");
	}

	function dendrites_read() { //Receive axon values from all neurons connected to dendrites
		if($this->sensor) return('s');
		$this->decisions = array(); //Remove old decisions
		foreach($this->synapses as $synapse => $strength) {
			//print_r (global_neuron($this->net, $synapse)); //debug
			global_neuron($this->net, $synapse)->dendrites_read();
			global_neuron($this->net, $synapse)->decide();
			$this->decisions[$synapse]=global_neuron($this->net, $synapse)->axon_get();
		}
	}

	function decide() { //Decide final axon (output) value - read dendrites first!
		if($this->sensor) return('s');
		$this->axon = 0; //Reset old
		foreach($this->decisions as $synapse => $decision) {
                	$this->axon += $decision * $this->synapses[$synapse];
                }
                if($this->axon > $this->treshold) {
                	$this->axon = 1;
                } else {
                	$this->axon = 0;
                }
	}
	
	function axon_get() {
		return $this->axon;
	}

	function learn($truth) { //Determine, what is truth
		if($this->sensor) return('s');
		foreach($this->decisions as $synapse => $decision) {
			if($decision == $truth) {
				$this->synapses[$synapse]++;
			} else {
				$this->synapses[$synapse]--;
			}
		}
	}
	
	function sensor_set($sensor_value, $is_sensor=true) { //Make sensor from this neuron and set axon value
		$this->sensor = $is_sensor;
		$this->axon = $sensor_value;
	}

}



class neural_network { //Single neural network class

	var $net = 'N/A'; //Name of this network
	var $neurons = array(); //Array of neuron objects

	function neural_network($net) { //Constructor - Set network name
		$this->net = $net;
	}

	function add_neuron($id, $treshold=0) { //Creates new neurone with specified id
		if(!isset($this->neurons[$id])) {
			$this->neurons[$id] = new neuron($this->net, $id, $treshold);
		} else return(0);
	}
	
	function debug() { //Print debug for all neurons in network
		echo("###########\nDebug information about $this->net network:\n###########\n\n");
		foreach($this->neurons as $neuron) $neuron->debug();
	}
	
	function learn($truth) { //Learn all neurons
		foreach($this->neurons as $neuron) $neuron->learn($truth);
	}

}

///BANNER
nervous_banner("\nNeural network simulator\nCoded by Harvie 2oo7/8\nInfo:\n- Version: $nmnn_version\n- NeuroNet library loaded!\n");

/*
		8x ---------------------------------- CUT HERE ----------------------------------
 */

///SAMPLE_NETWORK///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

neural_network_create('neuronet'); //Create new network called neuronet
for($i=1;$i<=6;$i++) neural_network('neuronet')->add_neuron($i,-1); //Create few neurons in neuronet

/*
	Now wee have six neurons floating somewhere in the network space:
	
	(1) (2) (3) (4) (5) (6)
 */

//Connect neurons into synapse infrastructure:
global_neuron('neuronet', 1)->synapse_request(array(3,4));
global_neuron('neuronet', 2)->synapse_request(array(4,5));
global_neuron('neuronet', 3)->synapse_request(6);
global_neuron('neuronet', 4)->synapse_request(6);
global_neuron('neuronet', 5)->synapse_request(6);

//Convert neurons 1 and 2 to sensors:
for($i=1;$i<=2;$i++) global_neuron('neuronet', $i)->sensor_set(0, true);


/*
	After making this synapse infrastructure, we'll get somethinq like this:
	
	                _(3)_	
	     /-->  ]1)_/     \
	Input          \_(4)__\_(6> -> Output
	     \-->  ]2)_/      /
	               \_(5)_/
	
	]1) and ]2) are sensors/receptors/...
	(6> is output
	
	Network structure inspired by: http://en.wikipedia.org/wiki/Image:Neural_network_example.png
	
	If you can't understand this, i can try to say it easier: "It's something like your brain (but much more complicated)" ;D
	Or try to read one of these:
		- http://en.wikipedia.org/wiki/Neural_networks
		- http://en.wikipedia.org/wiki/Artificial_neural_network
	
 */

//Now, i have neural network and i will try to teach it something

/*

	I will try to teach this network to compare two booleans
	Epected output values:
		INPUT	|	OUTPUT
		0,0	|	1
		1,1	|	1
		0,1	|	0
		1,0	|	0

	So. Let's try to write some function for this:
	
*/

function same($first, $second) { //This function uses neural network to compare two bools

	global_neuron('neuronet', 1)->sensor_set($first);
	global_neuron('neuronet', 2)->sensor_set($second);

	global_neuron('neuronet', 6)->dendrites_read();
	global_neuron('neuronet', 6)->decide();
	$decision = global_neuron('neuronet', 6)->axon_get();

	echo("$first == $second ?\nnetwork says: $decision (0=NO, 1=YES)\n");
	
	return($decision);
}

/*

	We got nice function (in fact it hardly sux, like whole this network...)
	How to use it?:

*/

///Learning:

same(0,0); //Ask
neural_network('neuronet')->learn('true'); //Correct answer (teach)
neural_network('neuronet')->learn('true'); //Correct answer (teach)
same(1,0);
neural_network('neuronet')->learn('false');
neural_network('neuronet')->learn('false');
same(0,1);
neural_network('neuronet')->learn('false');
neural_network('neuronet')->learn('false');

///Using superinteligent lifeform ehm... 3 neurones brain ;P

echo("\nI hope it will work now: 8')\n\n");
same(0,0); //Ask (without teaching)

//Now, we like to print some stats
neural_network('neuronet')->debug(); //Print some info about neuronet and it's neurons


