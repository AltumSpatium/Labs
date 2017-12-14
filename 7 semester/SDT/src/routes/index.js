import React from 'react';
import { Switch, Redirect } from 'react-router';
import PublicRoute from 'routes/PublicRoute';
import PrivateRoute from 'routes/PrivateRoute';

import App from 'components/App';
import Income from 'components/income-page/Income';
import Expenses from 'components/expenses-page/Expenses';
import Accounts from 'components/accounts-page/Accounts';

import Register from 'components/register-page/Register';
import Login from 'components/login-page/Login';

const routes = (
    <Switch>
        <PublicRoute exact path={Register.path} component={Register} />
        <PublicRoute exact path={Login.path} component={Login} />
        <Redirect exact from={App.path} to={Income.path} />
        <App>
            <PrivateRoute path={Income.path} component={Income} />
            <PrivateRoute path={Expenses.path} component={Expenses} />
            <PrivateRoute path={Accounts.path} component={Accounts} />
        </App>
    </Switch>
);

export default routes;
