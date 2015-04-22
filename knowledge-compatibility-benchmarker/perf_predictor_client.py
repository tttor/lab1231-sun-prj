#
#   Hello World client in Python
#   Connects REQ socket to tcp://localhost:5555
#   Sends "Hello" to server, expects "World" back
#

'''
This bridge the cpp-based prediction code (the caller) to the py-based prediction code
where the the prediction is actually carried out.
'''
import zmq

def predict(ann_id, ann_filepath, tmp_dir, port):
    context = zmq.Context()

    #  Socket to talk to server
    # print("Connecting to the Predictor server at port=%s"%port)
    socket = context.socket(zmq.REQ)
    socket.connect(port)

    #
    # print('Sending request...')
    request = ann_id+','+ann_filepath
    socket.send(request)

    #  Get the reply.
    # print ('Waiting for response...')
    response = socket.recv()
    # print('Received response= %s' % (response))

    ypred_filepath = tmp_dir+'/ypred.csv'
    with open(ypred_filepath, 'wb') as fo:
        fo.write(response)

def main():
    print('Testing...')
    ann_id = 'abcd'
    ann_filepath = '/abc/de'
    tmp_dir = 'qwe/rt'
    port = 'tcp://localhost:5555'
    predict(ann_id, ann_filepath, tmp_dir, port)

if __name__ == '__main__':
    main()
