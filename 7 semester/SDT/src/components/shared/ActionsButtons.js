import React from 'react';
import { Button } from 'semantic-ui-react';

const ActionsButtons = props => {
    const { onEditClick, onRemoveClick } = props;

    return (
        <div>
            <Button icon='edit' className='actions-btn edit-btn' onClick={onEditClick} />
            <Button negative icon='delete' className='actions-btn delete-btn' onClick={onRemoveClick} />
        </div>
    );
}

export default ActionsButtons;
