from sklearn.metrics import accuracy_score
from sklearn.metrics import precision_score
from sklearn.metrics import recall_score
from sklearn.metrics import fbeta_score
from sklearn.metrics import hamming_loss
from sklearn.metrics import confusion_matrix

def compute(y_true, y_pred):
    perf = {}
    
    perf['accuracy'] = accuracy_score(y_true, y_pred)
    perf['precision'] = precision_score(y_true, y_pred, average='micro')
    perf['recall'] = recall_score(y_true, y_pred, average='micro')
    perf['fbeta_score'] = fbeta_score(y_true, y_pred, average='macro', beta=1.0)
    perf['hamming_loss'] = hamming_loss(y_true, y_pred)
    perf['cm'] = confusion_matrix(y_true, y_pred)
    
    return perf
