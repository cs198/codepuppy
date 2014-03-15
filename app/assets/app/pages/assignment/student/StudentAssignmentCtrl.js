angular.module('codepuppy').controller('StudentAssignmentCtrl', ['$scope', '$routeParams', '$fileUploader', function($scope, $routeParams, $fileUploader) {
    $scope.message = "I'm an assignment page for the course " + $routeParams.courseID;
    $scope.message+= " and assignment " + $routeParams.assignmentID;

    $scope.uploadPath = '/#/' + $routeParams.courseID + '/' + $routeParams.assignmentID + '/submit';
}]);