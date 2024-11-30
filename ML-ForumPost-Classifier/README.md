# **ML Classifier**

## **Overview**
The ML Classifier project applies natural language processing and machine learning techniques to classify Piazza posts based on their content. Using the "bag of words" model, this program analyzes past posts to predict their topic or author.

I completed this project collaboratively with a partner, using provided starter code. My primary contributions focused on implementing functions in BinarySearchTree.h, developing the classifier logic, and optimizing its accuracy. Other components of the project were either part of the starter code or implemented by my partner.

## **Skills Demonstrated**
- **Natural Language Processing**: Implemented the "bag of words" model to extract meaningful features from textual data.
- **Machine Learning Basics**: Designed and trained a simplified Multi-Variate Bernoulli Naive Bayes Classifier.
- **Data Structures**: Utilized maps and sets to store, retrieve, and analyze training data efficiently.
- **Collaborative Development**: Efficiently communicated and split tasks with a partner to design, debug, and test the system.

## **Key Features**
- **Training**: Processes labeled data to calculate class probabilities and word likelihoods using training datasets.
- **Prediction**: Determines the most likely label for a new post based on computed log-probability scores.
- **Flexible Input**: Supports classification by topic (e.g., "exam," "euchre") or by author (e.g., "instructor," "student").
- **Robust Error Handling**: Provides clear messages for missing files or invalid arguments.

## **Example Output**

```plaintext
trained on 8 examples
vocabulary size = 49

test data:
  correct = euchre, predicted = euchre, log-probability score = -13.7
  content = my code segfaults when bob is the dealer

  correct = euchre, predicted = calculator, log-probability score = -12.5
  content = no rational explanation for this bug

  correct = calculator, predicted = calculator, log-probability score = -13.6
  content = countif function in stack class not working

performance: 2 / 3 posts predicted correctly
