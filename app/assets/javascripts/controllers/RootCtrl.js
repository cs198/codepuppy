angular.module('codepuppy').controller('RootCtrl', function($scope) {
	var FAKEclasses = ['cs106a', 'cs106b', 'cs106x', 'cs142'];
    $scope.classes = FAKEclasses;
});