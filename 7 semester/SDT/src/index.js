import React from 'react';
import { render } from 'react-dom';
import { Provider } from 'react-redux';
import { BrowserRouter as Router } from 'react-router-dom';
import configureStore from 'store/configureStore';

import routes from 'routes';

import 'styles/index.css';

const store = configureStore();

render(
    <Provider store={store}>
        <Router>
            {routes}
        </Router>
    </Provider>,
    document.getElementById('root')
);
