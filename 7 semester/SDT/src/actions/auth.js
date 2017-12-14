import firebaseApp from 'utils/firebase';
import {
    request,
    success,
    failure
} from 'actions/default';
import {
    LOGIN_REQUEST, LOGIN_SUCCESS, LOGIN_FAILURE,
    LOGOUT,
    REGISTER_REQUEST, REGISTER_SUCCESS, REGISTER_FAILURE
} from 'constants/auth';

const loginRequest = request(LOGIN_REQUEST);
const loginSuccess = success(LOGIN_SUCCESS);
const loginFailure = failure(LOGIN_FAILURE, err => { console.log('=(', err) });
const registerRequest = request(REGISTER_REQUEST);
const registerSuccess = success(REGISTER_SUCCESS);
const registerFailure = failure(REGISTER_FAILURE, err => { console.log('=(', err) });

export const login = (email, password) => dispatch => {
    dispatch(loginRequest());
    return firebaseApp.auth().signInWithEmailAndPassword(email, password)
        .then(user => dispatch(loginSuccess(user)))
        .catch(err => {
            console.log('111,', err);
            dispatch(loginFailure(err));
        });
};

export const logout = () => dispatch => dispatch(request(LOGOUT)());

export const register = (email, password) => dispatch => {
    dispatch(registerRequest());
    return firebaseApp.auth().createUserWithEmailAndPassword(email, password)
        .then(user => {
            dispatch(registerSuccess());
            return user;
        })
        .catch(err => {dispatch(registerFailure(err))});
};
