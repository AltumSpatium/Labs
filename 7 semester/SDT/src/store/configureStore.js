import { createStore, applyMiddleware, compose } from 'redux';
import rootReducer from 'reducers';
import { ping } from './enhancers/ping';
import thunk from 'redux-thunk';

export default function configureStore(intitialState) {
    const middlewares = [
        thunk,
        ping
    ];

    const composeEnhancers = window.__REDUX_DEVTOOLS_EXTENSION_COMPOSE__ || compose;

    const store = createStore(
        rootReducer,
        intitialState,
        composeEnhancers(
            applyMiddleware(...middlewares)
        )
    );

    if (module.hot) {
        module.hot.accept('reducers', () => {
            const nextRootReducer = require('reducers').default;
            store.replaceReducer(nextRootReducer);
        });
    }

    return store;
}
