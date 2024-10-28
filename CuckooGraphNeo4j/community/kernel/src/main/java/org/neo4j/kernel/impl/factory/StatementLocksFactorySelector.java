/*
 * Copyright (c) 2002-2020 "Neo4j,"
 * Neo4j Sweden AB [http://neo4j.com]
 *
 * This file is part of Neo4j.
 *
 * Neo4j is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
package org.neo4j.kernel.impl.factory;

import java.util.Collection;

import org.neo4j.configuration.Config;
import org.neo4j.internal.helpers.collection.Iterables;
import org.neo4j.kernel.impl.locking.Locks;
import org.neo4j.kernel.impl.locking.SimpleStatementLocksFactory;
import org.neo4j.kernel.impl.locking.StatementLocksFactory;
import org.neo4j.logging.Log;
import org.neo4j.logging.internal.LogService;
import org.neo4j.service.Services;
import org.neo4j.util.VisibleForTesting;

public class StatementLocksFactorySelector
{
    private final Locks locks;
    private final Config config;
    private final Log log;

    public StatementLocksFactorySelector( Locks locks, Config config, LogService logService )
    {
        this.locks = locks;
        this.config = config;
        this.log = logService.getInternalLog( getClass() );
    }

    public StatementLocksFactory select()
    {
        StatementLocksFactory statementLocksFactory;

        String serviceName = StatementLocksFactory.class.getSimpleName();
        Collection<StatementLocksFactory> factories = serviceLoadFactories();
        if ( factories.isEmpty() )
        {
            statementLocksFactory = new SimpleStatementLocksFactory();

            log.info( "No services implementing " + serviceName + " found. " +
                      "Using " + SimpleStatementLocksFactory.class.getSimpleName() );
        }
        else if ( factories.size() == 1 )
        {
            statementLocksFactory = Iterables.first( factories );

            log.info( "Found single implementation of " + serviceName +
                      ". Namely " + statementLocksFactory.getClass().getSimpleName() );
        }
        else
        {
            throw new IllegalStateException(
                    "Found more than one implementation of " + serviceName + ": " + factories );
        }

        statementLocksFactory.initialize( locks, config );

        return statementLocksFactory;
    }

    /**
     * Load all available factories via {@link Service}.
     *
     * @return list of available factories.
     */
    @VisibleForTesting
    Collection<StatementLocksFactory> serviceLoadFactories()
    {
        return Services.loadAll( StatementLocksFactory.class );
    }
}
