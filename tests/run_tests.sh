#!/bin/bash

# Script de test pour WIZZ Mania
# Teste la capacité du serveur à gérer plusieurs clients simultanément

echo "🧪 WIZZ Mania - Script de Test Automatique"
echo "=========================================="

# Configuration
NUM_CLIENTS=${1:-10}
DURATION=${2:-30}
SERVER_PORT=9999

echo "Configuration du test:"
echo "  - Nombre de clients: $NUM_CLIENTS"
echo "  - Durée du test: ${DURATION}s"
echo "  - Port du serveur: $SERVER_PORT"
echo ""

# Fonction de nettoyage
cleanup() {
    echo ""
    echo "🧹 Nettoyage des processus..."
    kill $SERVER_PID 2>/dev/null
    kill $CLIENT_PID 2>/dev/null
    wait $SERVER_PID 2>/dev/null
    wait $CLIENT_PID 2>/dev/null
    echo "✅ Nettoyage terminé"
}

# Gestionnaire de signaux pour le nettoyage
trap cleanup EXIT INT TERM

# Compiler les tests
echo "🔨 Compilation des programmes de test..."
cd "$(dirname "$0")"
mkdir -p build
cd build

if ! cmake ..; then
    echo "❌ Échec de la configuration CMake"
    exit 1
fi

if ! make; then
    echo "❌ Échec de la compilation"
    exit 1
fi

echo "✅ Compilation réussie"
echo ""

# Lancer le serveur en arrière-plan
echo "🚀 Démarrage du serveur de test..."
./test_server $SERVER_PORT &
SERVER_PID=$!

# Attendre que le serveur démarre
sleep 2

# Vérifier que le serveur fonctionne
if ! kill -0 $SERVER_PID 2>/dev/null; then
    echo "❌ Le serveur n'a pas pu démarrer"
    exit 1
fi

echo "✅ Serveur démarré (PID: $SERVER_PID)"
echo ""

# Lancer le test des clients
echo "👥 Démarrage du test des clients..."
./test_clients $NUM_CLIENTS $DURATION &
CLIENT_PID=$!

# Attendre la fin du test
wait $CLIENT_PID
TEST_RESULT=$?

echo ""
if [ $TEST_RESULT -eq 0 ]; then
    echo "✅ Test terminé avec succès !"
    echo "📊 Le serveur a géré $NUM_CLIENTS clients simultanément pendant ${DURATION}s"
else
    echo "❌ Le test a échoué"
fi

echo ""
echo "🎯 Résumé du test:"
echo "  - Clients simultanés: $NUM_CLIENTS"
echo "  - Durée: ${DURATION}s"
echo "  - Port: $SERVER_PORT"
echo "  - Statut: $([ $TEST_RESULT -eq 0 ] && echo 'RÉUSSI' || echo 'ÉCHEC')"

exit $TEST_RESULT