import torch
import torch.nn as nn
import torchvision.models as models


class EncoderCNN(nn.Module):
    def __init__(self, embed_size):
        super(EncoderCNN, self).__init__()
        resnet = models.resnet50(pretrained=True)
        for param in resnet.parameters():
            param.requires_grad_(False)
        
        modules = list(resnet.children())[:-1]
        self.resnet = nn.Sequential(*modules)
        self.embed = nn.Linear(resnet.fc.in_features, embed_size)

    def forward(self, images):
        features = self.resnet(images)
        features = features.view(features.size(0), -1)
        features = self.embed(features)
        return features
    

class DecoderRNN(nn.Module):
    def __init__(self, embed_size, hidden_size, vocab_size, num_layers=1):
        super(DecoderRNN, self).__init__()
        self.n_hidden = hidden_size
        self.embed_size = embed_size
        self.vocab_size = vocab_size
        self.embed = nn.Embedding(vocab_size, embed_size)
        
        self.lstm = nn.LSTM(embed_size, hidden_size, num_layers, batch_first = True)
        self.fc = nn.Linear(hidden_size, vocab_size)
          
        self.init_weights()
        
    def init_weights(self):
        torch.nn.init.xavier_uniform_(self.fc.weight)
        torch.nn.init.xavier_uniform_(self.embed.weight) 
        
    def forward(self, features, captions):
        captions = captions[:, :-1]
        captions = self.embed(captions)
        
        features = features.unsqueeze(1)
        inputs = torch.cat((features, captions), 1)
        outputs, _=self.lstm(inputs)
        
        outputs = self.fc(outputs)
        
        return outputs

    def sample(self, inputs, states=None, max_len=20):
        " accepts pre-processed image tensor (inputs) and returns predicted sentence (list of tensor ids of length max_len) "
        preds = []
        count = 0
        word_item = None
        
        while count < max_len and word_item != 1 :
            
            #Predict output
            output_lstm, states = self.lstm(inputs, states)
            output = self.fc(output_lstm)
            
            #Get max value
            prob, word = output.max(2)
            
            #append word
            word_item = word.item()
            preds.append(word_item)
            
            #next input is current prediction
            inputs = self.embed(word)
            
            count+=1
        
        return preds