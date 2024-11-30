# **ML Classifier**

## **Overview**
The ML Classifier project applies natural language processing and machine learning techniques to classify Piazza posts based on their content. Using the "bag of words" model, this program analyzes past posts to predict their topic or author. 

This project was completed collaboratively with a partner, using provided starter code. My primary contributions focused on implementing functions in BinarySearchTree.h, developing the classifier logic, and optimizing its accuracy. Other components of the project were either part of the starter code or implemented by my partner.

## **Skills Demonstrated**
- **Natural Language Processing**: Implemented the "bag of words" model to extract meaningful features from textual data.
- **Machine Learning Basics**: Designed and trained a simplified Multi-Variate Bernoulli Naive Bayes Classifier.
- **Data Structures**: Utilized maps and sets to store, retrieve, and analyze training data efficiently.
- **Collaborative Development**: Worked with a partner to design, debug, and test the system under strict project requirements.

## **Key Features**
- **Training**: Processes labeled data to calculate class probabilities and word likelihoods using training datasets.
- **Prediction**: Determines the most likely label for a new post based on computed log-probability scores.
- **Flexible Input**: Supports classification by topic (e.g., "exam," "euchre") or by author (e.g., "instructor," "student").
- **Robust Error Handling**: Provides clear messages for missing files or invalid arguments.

## **Example Workflow**

### **Training Information**
```plaintext
trained on 8 examples
vocabulary size = 49

classes:
  calculator, 3 examples, log-prior = -0.981
  euchre, 5 examples, log-prior = -0.470

classifier parameters:
  calculator:assert, count = 1, log-likelihood = -1.100
  euchre:upcard, count = 2, log-likelihood = -0.916
