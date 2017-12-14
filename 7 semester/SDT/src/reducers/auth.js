import { 
    LOGIN_REQUEST, LOGIN_SUCCESS, LOGIN_FAILURE,
    LOGOUT,
    REGISTER_REQUEST, REGISTER_SUCCESS, REGISTER_FAILURE,
} from 'constants/auth';

const initialState = {
    isLoading: false,
    user: null,
    error: ''
};

const auth = (state = initialState, action) => {
    switch (action.type) {
        // login actions
        case LOGIN_REQUEST:
            return { ...state, isLoading: true, error: '' };
        case LOGIN_SUCCESS:
            localStorage['stUser'] = JSON.stringify(action.payload);
            return { ...state, user: action.payload, isLoading: false };
        case LOGIN_FAILURE:
            localStorage.removeItem('stUser');
            return { ...state, isLoading: false, error: 'Logging error' };

        // logout action
        case LOGOUT:
            localStorage.removeItem('stUser');
            return { ...state, user: null };

        // register actions
        case REGISTER_REQUEST:
            return { ...state, isLoading: true, error: '' };
        case REGISTER_SUCCESS:
            localStorage['stUser'] = JSON.stringify(action.payload);
            return { ...state, user: action.payload, isLoading: false };
        case REGISTER_FAILURE:
            return { ...state, isLoading: false, error: 'Registration error' };

        default:
            return state;
    }
};

export default auth;
