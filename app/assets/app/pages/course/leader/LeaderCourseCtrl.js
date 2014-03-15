angular.module('codepuppy').controller('LeaderCourseCtrl', ['$scope', '$routeParams', function($scope, $routeParams) {
    $scope.message = 'Look! I am a course page for ' + $routeParams.courseID;
}]);
