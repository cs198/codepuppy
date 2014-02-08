angular.module('codepuppy').controller('ClassCtrl', function($scope, $routeParams) {
    $scope.message = 'Look! I am a class page for ' + $routeParams.classID;
    $scope.$parent.selectedClass = $routeParams.classID;
    $scope.$parent.selectedAssignment = null;

    var FAKEassignmentsForGivenClass = ['One', 'Two']
    $scope.$parent.assignments = FAKEassignmentsForGivenClass;
});