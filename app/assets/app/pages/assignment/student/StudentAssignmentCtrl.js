angular.module('codepuppy').controller('StudentAssignmentCtrl', ['$scope',
    '$routeParams', '$fileUploader', '$http',
    function($scope, $routeParams, $fileUploader, $http) {

  var getAssignment = function() {
    $http({
      method: 'GET',
      url: '/assignments/' + $routeParams.assignmentID + '.json'
    })
    .success(function(data, status, headers, config) {
      if (data.name) {
        $scope.assignmentName = data.name;
      } else {
        $scope.assignmentName = "";
      }
      if (data.description) {
        $scope.assignmentDescription = ParseWiki(data.description);
      } else {
        $scope.assignmentDescription = "";
      }
    });
  };
  getAssignment();

  $scope.message = "I'm an assignment page for the course " + $routeParams.courseID;
  $scope.message+= " and assignment " + $routeParams.assignmentID;

  $scope.uploadPath = '/#/' + $routeParams.courseID + '/' + $routeParams.assignmentID + '/submit';
}]);
