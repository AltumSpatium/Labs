import React from 'react';

const nameHeader = className => ({
    Header: 'Name', accessor: 'name',
    Cell: props => <div className={className}>{props.value}</div>
});
const categoryHeader = className => ({
    Header: 'Category', accessor: 'category',
    Cell: props => <div className={className}>{props.value}</div>
});
const dateHeader = className => ({
    Header: 'Date', accessor: 'date',
    Cell: props => <div className={className}>{props.value}</div>
});
const moneyHeader = className => ({
    Header: 'Money', accessor: 'displayMoney',
    Cell: props => <div className={className}>{props.value}</div>
});
const accountHeader = className => ({
    Header: 'Account', accessor: 'displayAccount',
    Cell: props => <div className={className}>{props.value}</div>
});
const actionsHeader = { 
    Header: 'Actions', accessor: 'actions',
    Cell: props => <div className='actions-cell'>{props.value}</div>
};
const titleHeader = className => ({
    Header: 'Title', accessor: 'title',
    Cell: props => <div className={className}>{props.value}</div>
});
const typeHeader = className => ({
    Header: 'Type', accessor: 'type',
    Cell: props => <div className={className}>{props.value}</div>
});

export const linkSet = {
    GUESTS_LINKS: [
        { path: '/login', name: 'Login' },
        { path: '/register', name: 'Register' },
    ],

    USERS_LINKS: [
        { path: '/income', name: 'Income' },
        { path: '/expenses', name: 'Expenses' },
        { path: '/accounts', name: 'Accounts' },
    ],
};

export const tableHeaders = {
    INCOME: [
        nameHeader('cell cell-center'),
        categoryHeader('cell cell-center'),
        dateHeader('cell cell-center'),
        moneyHeader('cell cell-center cell-money-green'),
        accountHeader('cell cell-center'),
        actionsHeader
    ],

    EXPENSES: [
        nameHeader('cell cell-center'),
        categoryHeader('cell cell-center'),
        dateHeader('cell cell-center'),
        moneyHeader('cell cell-center cell-money-red'),
        accountHeader('cell cell-center'),
        actionsHeader
    ],

    ACCOUNTS: [
        titleHeader('cell cell-center'),
        typeHeader('cell cell-center'),
        moneyHeader('cell cell-center'),
        actionsHeader
    ]
};
