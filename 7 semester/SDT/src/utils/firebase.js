import firebase from 'firebase';

const config = {
  apiKey: "AIzaSyB-tYHnWazXvyenC34TAgvc70G3Kjp-_6I",
  authDomain: "shekeltracker.firebaseapp.com",
  databaseURL: "https://shekeltracker.firebaseio.com",
  projectId: "shekeltracker",
  storageBucket: "shekeltracker.appspot.com",
  messagingSenderId: "877180638048"
};

const firebaseApp = firebase.initializeApp(config);
export default firebaseApp;
