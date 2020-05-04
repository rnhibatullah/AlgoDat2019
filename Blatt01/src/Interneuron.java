import java.util.ArrayList;

/**
 * The class Neuron implents a interneuron for the class Network.
 *
 * @author Vera Röhr
 * @version 1.0
 * @since 2019-01-11
 */
public class Interneuron extends Neuron {
    /**
     * {@inheritDoc}
     */
    public Interneuron(int index) {
        // TODO
        super(index);
        outgoingsynapses = new ArrayList<Synapse>();
    }

    /**
     * Divides incoming signal into equal parts for all the outgoing synapses
     *
     * @param input 3 dimensional signal from another neuron
     * @return 3 dimensional neural signal (after processing)
     */
    @Override
    public double[] integrateSignal(double[] signal) {
        // TODO
        //int numberOfSynapse = this.outgoingsynapses.size();
        int numberOfSignal = signal.length;
        //
        for (int i =0; i<numberOfSignal; i++){
            signal[i] = signal[i]/this.outgoingsynapses.size();
        }
        //transmit the Signal
        for (int g= 0; g <this.outgoingsynapses.size(); g++){
            this.outgoingsynapses.get(g).transmit(signal);
        }

        return signal;
    }
}

